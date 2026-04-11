#include <cstdio>
#include <cstring>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

#include "AST.hh"
#include "checksieve.h"
#include "diagnostic.hh"
#include "EmailMessage.hh"
#include "sieve_driver.hh"
#include "MailServer.hh"

const char *usage_string  =
"Usage: check-sieve [options] file1 [file2 ...]                                 \n"
"                                                                               \n"
"Options:                                                                       \n"
"  -h, --help               Show this message                                   \n"
"  --max-list-length N      Flag lists over a certain length (default: none)    \n"
"  --server <host>:<port>   Only allow capabilities advertised by the specified \n"
"                           server.                                             \n"
"  --trace-parser           Trace the operation of the parser                   \n"
"  --trace-scanner          Trace the operation of the scanner                  \n"
"  --trace-tree             Trace the abstract-syntax-tree                      \n"
"  --simulate <email-file>  Simulate sieve processing against an email (.eml)  \n"
"  --test-dir <dir>         Run simulation for all .eml files in dir and diff  \n"
"                           against matching .out files; exit 1 if any differ  \n"
"  --rebase <dir>           Regenerate all .out files in dir from current      \n"
"                           simulation output                                  \n"
"  --version                Print out version information                       \n";

void print_version() {
    std::cout << "check-sieve, version " << sieve::version() << " (" << PLATFORM << ")" << std::endl;
}

void print_help() {
    std::cout << usage_string << std::endl;
}

bool file_exists(const char *filename) {
    std::ifstream file(filename);
    return (bool)file;
}

int main( int argc, char *argv[] ) {
    int result = 0;
    bool trace_scanning = false;
    bool trace_parsing = false;
    bool trace_tree = false;
    std::string simulate_email_path;
    std::string test_dir_path;
    std::string rebase_dir_path;
    std::vector<std::string> sieve_files;
    sieve::Diagnostic diag;
    struct sieve::parse_options options;

    if (argc == 1) {
        print_help();
        return 1;
    }

    // First pass: parse all options and collect sieve file paths
    for (int i = 1; i < argc; ++i) {
        // Long argument
        if (argv[i][0] == '-' && argv[i][1] == '-') {
            if (strcmp(argv[i], "--max-list-length") == 0) {
                if (i + 1 >= argc) {
                    std::cerr << "Expected a number after --max-list-length." << std::endl;
                    return 1;
                }
                options.string_list_max_length = atoi(argv[i+1]);
                i++;
                continue;
            }

            if (strcmp(argv[i], "--trace-scanner") == 0) {
                trace_scanning = true;
                continue;
            }

            if (strcmp(argv[i], "--trace-parser") == 0) {
                trace_parsing = true;
                continue;
            }

            if (strcmp(argv[i], "--trace-tree") == 0) {
                trace_tree = true;
                continue;
            }

            if (strcmp(argv[i], "--help") == 0) {
                print_help();
                return 0;
            }

            if (strcmp(argv[i], "--version") == 0) {
                print_version();
                return 0;
            }

            if (strcmp(argv[i], "--simulate") == 0) {
                if (i + 1 >= argc) {
                    std::cerr << "Expected an email file path after --simulate." << std::endl;
                    return 1;
                }
                simulate_email_path = argv[i+1];
                i++;
                continue;
            }

            if (strcmp(argv[i], "--test-dir") == 0) {
                if (i + 1 >= argc) {
                    std::cerr << "Expected a directory path after --test-dir." << std::endl;
                    return 1;
                }
                test_dir_path = argv[i+1];
                i++;
                continue;
            }

            if (strcmp(argv[i], "--rebase") == 0) {
                if (i + 1 >= argc) {
                    std::cerr << "Expected a directory path after --rebase." << std::endl;
                    return 1;
                }
                rebase_dir_path = argv[i+1];
                i++;
                continue;
            }

            if (strcmp(argv[i], "--server") == 0) {
                if (i + 1 >= argc)
                {
                    std::cerr << "Expected a connection string after --server" << std::endl;
                    return 1;
                }

                auto server = sieve::MailServer::create(argv[i+1]);
                options.all_supported_capabilities = false;
                options.capabilities = server.capabilities();
                i++;
                continue;
            }

            std::cerr << "Unrecognized argument \"" << argv[i] << "\"" << std::endl;
            return 1;
        }
        // Short argument
        else if (argv[i][0] == '-') {
            if (strcmp(argv[i], "-h") == 0) {
                print_help();
                return 0;
            }
        }
        // Sieve file
        else {
            sieve_files.push_back(argv[i]);
        }
    }

    // Second pass: process sieve files (skipped when --test-dir or --rebase is used)
    for (const auto &sieve_file : (!test_dir_path.empty() || !rebase_dir_path.empty() ? std::vector<std::string>{} : sieve_files)) {
        if (!file_exists(sieve_file.c_str())) {
            std::cerr << "Could not find file \"" << sieve_file << "\"." << std::endl;
            result = 2;
            break;
        }

        sieve::driver driver(options);

        // Configure driver
        driver.trace_scanning = trace_scanning;
        driver.trace_parsing = trace_parsing;
        driver.trace_tree = trace_tree;

        sieve::parse_result parse_res = driver.parse_file( sieve_file );

        if ( !parse_res.status ) {
            if (!simulate_email_path.empty()) {
                if (!file_exists(simulate_email_path.c_str())) {
                    std::cerr << "Could not find email file \"" << simulate_email_path << "\"." << std::endl;
                    result = 2;
                    break;
                }
                sieve::EmailMessage email = sieve::EmailMessage::parse(simulate_email_path);
                sieve::ASTSimulationVisitor sim(email, sieve_file, simulate_email_path);
                sim.walk(driver.syntax_tree());
            } else if (driver.trace_tree) {
                sieve::ASTTraceVisitor visitor = sieve::ASTTraceVisitor();
                visitor.walk(driver.syntax_tree());
            } else {
                std::cout << sieve_file << ": No errors found!" << std::endl;
            }
        } else {
            std::ifstream fin( sieve_file );
            std::cerr << "Errors found in \"" << sieve_file << "\":" << std::endl << std::endl;
            std::cerr << diag.describe(parse_res, fin);
            result = 1;
        }
    }

    if (!test_dir_path.empty() || !rebase_dir_path.empty()) {
        const std::string &dir = !test_dir_path.empty() ? test_dir_path : rebase_dir_path;

        if (sieve_files.size() != 1) {
            std::cerr << "--test-dir/--rebase requires exactly one sieve file argument." << std::endl;
            return 1;
        }
        const std::string &sieve_file = sieve_files[0];

        sieve::driver driver(options);
        driver.trace_scanning = trace_scanning;
        driver.trace_parsing = trace_parsing;
        driver.trace_tree = trace_tree;
        sieve::parse_result parse_res = driver.parse_file(sieve_file);
        if (parse_res.status) {
            std::ifstream fin(sieve_file);
            std::cerr << "Errors found in \"" << sieve_file << "\":" << std::endl << std::endl;
            std::cerr << diag.describe(parse_res, fin);
            return 1;
        }

        int failures = 0, total = 0;

        DIR *dp = opendir(dir.c_str());
        if (!dp) {
            std::cerr << "Cannot open directory: " << dir << std::endl;
            return 1;
        }
        struct dirent *de;
        while ((de = readdir(dp)) != nullptr) {
            std::string name = de->d_name;
            if (name.size() < 4 || name.substr(name.size() - 4) != ".eml") continue;
            total++;
            std::string eml_path = dir + "/" + name;

            // Capture simulation output
            std::ostringstream captured;
            std::streambuf *orig = std::cout.rdbuf(captured.rdbuf());
            sieve::EmailMessage email = sieve::EmailMessage::parse(eml_path);
            sieve::ASTSimulationVisitor sim(email, sieve_file, eml_path);
            sim.walk(driver.syntax_tree());
            std::cout.rdbuf(orig);

            // Strip first line (environment-specific path line)
            std::string output = captured.str();
            auto nl = output.find('\n');
            std::string stripped = (nl != std::string::npos) ? output.substr(nl + 1) : output;

            std::string out_path = eml_path.substr(0, eml_path.size() - 4) + ".out";

            if (!rebase_dir_path.empty()) {
                std::ofstream out_file(out_path);
                out_file << stripped;
            } else {
                std::string expected;
                struct stat st;
                bool out_exists = (stat(out_path.c_str(), &st) == 0);
                if (out_exists) {
                    std::ifstream in(out_path);
                    expected.assign((std::istreambuf_iterator<char>(in)),
                                     std::istreambuf_iterator<char>());
                }
                if (stripped != expected) {
                    failures++;
                    char tmpname[] = "/tmp/check-sieve-XXXXXX";
                    int fd = mkstemp(tmpname);
                    if (write(fd, stripped.c_str(), stripped.size()) < 0) {
                        std::cerr << "Failed writing to temporary file at "
                                  << tmpname << ": "
                                  << std::strerror(errno) << "\n";
                        abort();
                    }
                    close(fd);
                    std::string expected_arg = out_exists ? out_path : "/dev/null";
                    std::string cmd = "diff -ubBd " + expected_arg + " " + std::string(tmpname);
                    FILE *pipe = popen(cmd.c_str(), "r");
                    char buf[256];
                    while (fgets(buf, sizeof(buf), pipe)) std::cout << buf;
                    pclose(pipe);
                    unlink(tmpname);
                }
            }
        }
        closedir(dp);

        if (!rebase_dir_path.empty()) {
            return 0;
        }
        if (failures > 0) {
            std::cerr << failures << " of " << total << " sort(s) failed." << std::endl;
            return 1;
        }
        return 0;
    }

    return result;
}

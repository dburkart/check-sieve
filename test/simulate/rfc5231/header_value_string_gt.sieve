require ["relational", "fileinto", "comparator-i;ascii-casemap"];
# Subject: "Hello World" — is "hello world" > "m"? no → no match, implicit keep
if header :value "gt" :comparator "i;ascii-casemap"
          ["subject"] ["M"]
{
    fileinto "From N-Z";
}

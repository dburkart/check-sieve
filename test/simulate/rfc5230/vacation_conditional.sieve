require "vacation";

if header :is "From" "Alice <alice@example.com>" {
    vacation "I am away on vacation.";
}

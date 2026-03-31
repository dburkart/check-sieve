require ["relational", "fileinto", "comparator-i;ascii-casemap"];
# From: alice@example.com :all = "alice@example.com" — is it > "M"? yes → match
if address :value "gt" :all :comparator "i;ascii-casemap"
           ["from"] ["M"]
{
    fileinto "From N-Z";
}

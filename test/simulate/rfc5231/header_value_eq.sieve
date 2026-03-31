require ["relational", "fileinto", "comparator-i;ascii-casemap"];
# Subject: "Hello World" — is "hello world" eq "hello world"? yes → match
if header :value "eq" :comparator "i;ascii-casemap"
          ["subject"] ["Hello World"]
{
    fileinto "Exact";
}

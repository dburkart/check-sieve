require ["relational", "fileinto", "comparator-i;ascii-casemap"];
# Subject: "Hello World" — is "hello world" ne "goodbye"? yes → match
if header :value "ne" :comparator "i;ascii-casemap"
          ["subject"] ["Goodbye"]
{
    fileinto "NotGoodbye";
}

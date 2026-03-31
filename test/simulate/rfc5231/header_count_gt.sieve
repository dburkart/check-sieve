require ["relational", "fileinto", "comparator-i;ascii-numeric"];
# basic.eml has 1 To header — is count(to) > 2? no → no match, implicit keep
if header :count "gt" :comparator "i;ascii-numeric"
          ["to"] ["2"]
{
    fileinto "SPAM";
}

require ["relational", "fileinto", "comparator-i;ascii-numeric"];
# basic.eml has 1 To header — is count(to) == 1? yes → match
if header :count "eq" :comparator "i;ascii-numeric"
          ["to"] ["1"]
{
    fileinto "SingleRecipient";
}

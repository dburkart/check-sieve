require ["relational", "fileinto", "comparator-i;ascii-numeric"];
# basic.eml has 1 To address — is count(to) == 1? yes → match
if address :count "eq" :comparator "i;ascii-numeric"
           ["to"] ["1"]
{
    fileinto "SingleRecipient";
}

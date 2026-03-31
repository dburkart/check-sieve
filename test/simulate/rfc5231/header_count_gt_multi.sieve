require ["relational", "fileinto", "comparator-i;ascii-numeric"];
# multi_to.eml has 3 To headers — is count(to) > 2? yes → match
if header :count "gt" :comparator "i;ascii-numeric"
          ["to"] ["2"]
{
    fileinto "SPAM";
}

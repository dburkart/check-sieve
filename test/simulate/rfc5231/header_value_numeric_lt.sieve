require ["relational", "fileinto", "comparator-i;ascii-numeric"];
# X-Priority: 2 — is 2 < 3? yes → match
if header :value "lt" :comparator "i;ascii-numeric"
          ["x-priority"] ["3"]
{
    fileinto "HighPriority";
}

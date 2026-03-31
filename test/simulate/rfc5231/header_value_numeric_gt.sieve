require ["relational", "fileinto", "comparator-i;ascii-numeric"];
# X-Priority: 2 — is 2 > 5? no → no match, implicit keep
if header :value "gt" :comparator "i;ascii-numeric"
          ["x-priority"] ["5"]
{
    fileinto "LowPriority";
}

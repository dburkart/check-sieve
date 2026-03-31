require ["relational", "fileinto", "comparator-i;ascii-numeric"];
# X-Priority: 5 in multi_to.eml — is 5 >= 5? yes → match
if header :value "ge" :comparator "i;ascii-numeric"
          ["x-priority"] ["5"]
{
    fileinto "LowPriority";
}

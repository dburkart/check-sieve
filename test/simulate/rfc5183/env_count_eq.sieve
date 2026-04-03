require ["environment", "relational", "fileinto", "comparator-i;ascii-numeric"];
# "domain" exists once — count should equal 1
if environment :count "eq" :comparator "i;ascii-numeric" "domain" ["1"] {
    fileinto "KnownItem";
}

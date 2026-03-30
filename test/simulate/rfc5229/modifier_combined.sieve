# RFC 5229 §4 — multiple modifiers applied in precedence order.
# :lower (40) applied before :upperfirst (30), so result is "Hello".
require ["variables", "fileinto"];

set :upperfirst :lower "tag" "WORLD";
fileinto "${tag}";

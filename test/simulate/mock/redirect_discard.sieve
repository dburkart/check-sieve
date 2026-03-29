require "fileinto";

if exists "X-Spam-Score" {
    redirect "admin@example.com";
} else {
    discard;
}

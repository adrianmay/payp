cat tags | grep "f$" | grep -v 'static' | grep  -Po '/\^\K[^{$]*' | sed 's#$#;#' > forwards.hh

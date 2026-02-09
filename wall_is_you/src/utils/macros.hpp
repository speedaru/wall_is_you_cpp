#pragma once
#define INIT_ONCE(accessSpecifier, code) static int _ = [accessSpecifier](){ code; return 0; }()
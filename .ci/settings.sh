BUNDLE_VERSION=0.3

GLOBAL_PREFIX="${SEMAPHORE_CACHE_DIR}/prefix"
APT_CACHE="${SEMAPHORE_CACHE_DIR}/apt"

GLIB_PREFIX="${GLOBAL_PREFIX}/glib"
GLIB_BUILD="/tmp/glib-build"

JSON_GLIB_PREFIX="${GLOBAL_PREFIX}/json-glib"
JSON_GLIB_BUILD="/tmp/json-glib-build"

GLIB_VERSIONS=( 2.35.9 2.40.2 2.44.0 )
JSON_GLIB_VERSIONS=( 0.16.2 1.0.2 )
BLACKLIST=( 2.35.9/1.0.2 )

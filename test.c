#include <assert.h>

#include "atlr.h"

b32 test_inverse_matrix() {
    u32 mem_size = 10 * ATLR_KILOBYTE;
    AtlrArena main_memory = atlr_mem_create_arena(malloc(mem_size), mem_size, "main");
    atlr_init(&main_memory);

    Matrix2x2 identity = {
        1, 0,
        0, 1
    };
    Matrix2x2 base = {
        30, 15,
        45, 70
    };
    Matrix2x2 inv_base = atlr_algebra_m2x2_inverse(base);
    Matrix2x2 verif = atlr_algebra_m2x2_mult(base, inv_base);
    f64 det = atlr_algebra_m2x2_determinant(base);
    f64 inv_det = atlr_algebra_m2x2_determinant(inv_base);
    f64 verif_det = atlr_algebra_m2x2_determinant(verif);

    f64 result = (det * inv_det) - verif_det;
    if ((s64) result != 0) {
        atlr_log_debug("FAILED: %f == %f", (det * inv_det), verif_det);
        return 0;
    }
    if (!atlr_algebra_m2x2_equal(verif, identity)) {
        atlr_log_debug("FAILED identity");
        atlr_log_debug("[%f,%f] [%f,%f]", identity.values[0], identity.values[1], identity.values[2], identity.values[3]);
        atlr_log_debug("[%f,%f] [%f,%f]", verif.values[0], verif.values[1], verif.values[2], verif.values[3]);
        return 0;
    }
    return 1;
}

int main() {
    assert(test_inverse_matrix());
    atlr_log_debug("SUCCESS: test_inverse_matrix");
    return 0;
}

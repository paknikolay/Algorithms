namespace ahalaiMahalai {
    struct Two {
        static constexpr size_t two = 1 << 1;
    };


//////////////////////////

    template<size_t MASK_LENGTH, size_t MASK, size_t MASK_END>//end = 0
    struct MaskCheckerOr {
        constexpr static bool isValid =
                MaskCheckerOr<MASK_LENGTH - Two::two, (MASK >> Two::two), (MASK >> Two::two) % Two::two>::isValid &&
                !((MASK >> 1) % Two::two);
    };


    template<size_t MASK_LENGTH, size_t MASK>//end =  1
    struct MaskCheckerOr<MASK_LENGTH, MASK, 1> {
        constexpr static bool isValid = MaskCheckerOr<MASK_LENGTH - 1, (MASK >> 1), (MASK >> 1) % Two::two>::isValid;
    };


    template<>
    struct MaskCheckerOr<0, 0, 0> {
        constexpr static bool isValid = true;
    };

    template<>
    struct MaskCheckerOr<1, 0, 0> {
        constexpr static bool isValid = false;
    };
//////////////////////////

    template<size_t MASK_LENGTH, size_t MASK_FROM, size_t MASK_TO>
    struct MaskChecker {
        constexpr static bool isValid = !(MASK_FROM & MASK_TO) && MaskCheckerOr<MASK_LENGTH, (MASK_TO | MASK_FROM),
                (MASK_TO | MASK_FROM) % Two::two>::isValid;
    };
///////////////////
    template<size_t M, size_t CURR_N, size_t MASK>
    struct MaskFrom;
////////////////////
    template<size_t M, size_t CURR_N, size_t MASK_FROM, size_t CUR_MASK>
    struct MaskToCycle {
        constexpr static size_t value = (MaskChecker<M, MASK_FROM, CUR_MASK>::isValid
                                         * MaskFrom<M, CURR_N - 1, CUR_MASK>::value
                                         + MaskToCycle<M, CURR_N, MASK_FROM, CUR_MASK - 1>::value) % MODULUS;
    };

    template<size_t M, size_t CURR_N, size_t MASK_FROM>
    struct MaskToCycle<M, CURR_N, MASK_FROM, 0> {
        constexpr static size_t value = (MaskChecker<M, 0, MASK_FROM>::isValid
                                         * MaskFrom<M, CURR_N - 1, 0>::value
                                        ) % MODULUS;
    };
//////////////////////////////////
    template<size_t M, size_t CURR_N, size_t MASK>
    struct MaskFrom {
        constexpr static size_t value = MaskToCycle<M, CURR_N, MASK, (1 << M) - 1>::value;
    };

    template<size_t M>
    struct MaskFrom<M, 0, 0> {
        constexpr static size_t value = 1;
    };


    template<size_t M, size_t MASK>
    struct MaskFrom<M, 0, MASK> {
        constexpr static size_t value = 0;
    };
/////////////////////////////////
    template<size_t M, size_t N, size_t CURRENT_N, size_t CURRENT_MASK>
    struct Calculator {
        constexpr static size_t value = (MaskCheckerOr<M, CURRENT_MASK, CURRENT_MASK % Two::two>::isValid
                                         * MaskFrom<M, CURRENT_N, CURRENT_MASK>::value
                                         + Calculator<M, N, CURRENT_N, CURRENT_MASK - 1>::value) % MODULUS;//TODO
    };
    template<size_t M, size_t N, size_t CURRENT_N>
    struct Calculator<M, N, CURRENT_N, 0> {
        constexpr static size_t value = (MaskCheckerOr<M, 0, 0>::isValid
                                         * MaskFrom<M, CURRENT_N, 0>::value);
    };

/////////////////////////////////////
    template<size_t M, size_t N, size_t M_MOD_2, size_t N_MOD_2>
    struct ParityChecker {
        constexpr static size_t value = Calculator<M, N, N - 1, (1 << M) - 1>::value;
    };

    template<size_t M, size_t N>
    struct ParityChecker<M, N, 1, 1> {
        constexpr static size_t value = 0;
    };


    template<size_t M, size_t N>
    struct AhalaiMahalai_helper {
        constexpr static size_t value = ParityChecker<M, N, M % Two::two, N % Two::two>::value;
    };
}
template <size_t M, size_t N>
using AhalaiMahalai = ahalaiMahalai::AhalaiMahalai_helper<M, N>;
//
// Created by Nico Russo on 4/24/26.
//

#ifndef GRAPHITE_FONT_OMNI_H
#define GRAPHITE_FONT_OMNI_H
#include <cstdint>

namespace OmniFont {
    struct CharData {
        uint8_t char_width;
        uint8_t char_height;
        uint8_t baseline;
        uint8_t move_width;
        const uint8_t* char_data;
    };

    constexpr uint8_t a_cap_data[] = {
        0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 2, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 2, 1, 0,
        0, 2, 2, 0, 2, 0,
        0, 0, 0, 0, 0, 0
    };

    constexpr CharData a_cap {
        6, 9, 8, 5, a_cap_data
    };

    constexpr uint8_t b_cap_data[] = {
        0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 2, 1, 0,
        0, 1, 1, 1, 2, 0,
        0, 1, 1, 2, 1, 0,
        0, 1, 1, 0, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 2, 2, 2, 2, 0,
        0, 0, 0, 0, 0, 0
    };

    constexpr CharData b_cap {
        6, 9, 8, 5, b_cap_data
        };

    constexpr uint8_t c_cap_data[] = {
        0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 2, 2, 0,
        0, 1, 1, 0, 0, 0,
        0, 1, 1, 0, 0, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 2, 2, 2, 2, 0,
        0, 0, 0, 0, 0, 0,
    };

    constexpr CharData c_cap {
        6, 9, 8, 5, c_cap_data
        };

    constexpr uint8_t d_cap_data[] = {
        0, 0, 0, 0, 0, 3,
        0, 1, 1, 1, 0, 0,
        0, 1, 1, 2, 1, 0,
        0, 1, 1, 0, 1, 0,
        0, 1, 1, 0, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 2, 2, 2, 2, 0,
        0, 0, 0, 0, 0, 0,
    };

    constexpr CharData d_cap {
        6, 9, 8, 5, d_cap_data
        };

    constexpr uint8_t e_cap_data[] = {
        0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 2, 2, 0,
        0, 1, 1, 1, 0, 0,
        0, 1, 1, 2, 0, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 2, 2, 2, 2, 0,
        0, 0, 0, 0, 0, 0,
    };

    constexpr CharData e_cap {
        6, 9, 8, 5, e_cap_data
        };

    constexpr uint8_t f_cap_data[] = {
        0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 2, 2, 0,
        0, 1, 1, 1, 0, 0,
        0, 1, 1, 2, 0, 3,
        0, 1, 1, 0, 0, 3,
        0, 2, 2, 0, 3, 3,
        0, 0, 0, 0, 3, 3
    };

    constexpr CharData f_cap {
        6, 9, 8, 5, f_cap_data
        };

    constexpr uint8_t g_cap_data[] = {
        0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 2, 2, 0,
        0, 1, 1, 0, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 2, 2, 2, 2, 0,
        0, 0, 0, 0, 0, 0
    };

    constexpr CharData g_cap {
        6, 9, 8, 5, g_cap_data
        };

    constexpr uint8_t h_cap_data[] = {
        0, 0, 0, 0, 0, 0,
        0, 1, 1, 0, 1, 0,
        0, 1, 1, 0, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 2, 1, 0,
        0, 1, 1, 0, 1, 0,
        0, 2, 2, 0, 2, 0,
        0, 0, 0, 0, 0, 0,
    };

    constexpr CharData h_cap {
        6, 9, 8, 5, h_cap_data
        };

    constexpr uint8_t i_cap_data[] = {
        0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 2, 1, 1, 2, 0,
        0, 0, 1, 1, 0, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 2, 2, 2, 2, 0,
        0, 0, 0, 0, 0, 0,
    };

    constexpr CharData i_cap {
        6, 9, 8, 5, i_cap_data
        };

    constexpr uint8_t j_cap_data[] = {
        0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 2, 1, 1, 2, 0,
        0, 0, 1, 1, 0, 0,
        0, 1, 1, 1, 0, 3,
        0, 1, 1, 1, 0, 3,
        0, 2, 2, 2, 0, 3,
        0, 0, 0, 0, 0, 3,
    };

    constexpr CharData j_cap {
        6, 9, 8, 5, j_cap_data
        };

    constexpr uint8_t k_cap_data[] = {
        0, 0, 0, 0, 0, 0,
        0, 1, 1, 0, 1, 0,
        0, 1, 1, 0, 1, 0,
        0, 1, 1, 1, 2, 0,
        0, 1, 1, 2, 1, 0,
        0, 1, 1, 0, 1, 0,
        0, 1, 1, 0, 1, 0,
        0, 2, 2, 0, 2, 0,
        0, 0, 0, 0, 0, 0,
    };

    constexpr CharData k_cap {
        6, 9, 8, 5, k_cap_data
        };

    constexpr uint8_t l_cap_data[] = {
        0, 0, 0, 0, 3, 3,
        0, 1, 1, 0, 3, 3,
        0, 1, 1, 0, 3, 3,
        0, 1, 1, 0, 3, 3,
        0, 1, 1, 0, 3, 3,
        0, 1, 1, 0, 0, 0,
        0, 1, 1, 1, 1, 0,
        0, 2, 2, 2, 2, 0,
        0, 0, 0, 0, 0, 0,
    };

    constexpr CharData l_cap {
        6, 9, 8, 5, l_cap_data
        };

    constexpr uint8_t m_cap_data[] = {
        0, 0, 0, 0, 3, 0, 0, 0,
        0, 1, 1, 0, 0, 0, 1, 0,
        0, 1, 1, 1, 0, 1, 1, 0,
        0, 1, 1, 1, 1, 1, 1, 0,
        0, 1, 1, 2, 1, 2, 1, 0,
        0, 1, 1, 0, 2, 0, 1, 0,
        0, 1, 1, 0, 0, 0, 1, 0,
        0, 2, 2, 0, 3, 0, 2, 0,
        0, 0, 0, 0, 3, 0, 0, 0
    };

    constexpr CharData m_cap {
        8, 9, 8, 7, m_cap_data
        };

    constexpr uint8_t n_cap_data[] = {
        0, 0, 0, 0, 0, 0, 0,
        0, 1, 1, 0, 0, 1, 0,
        0, 1, 1, 1, 0, 1, 0,
        0, 1, 1, 1, 1, 1, 0,
        0, 1, 1, 2, 1, 1, 0,
        0, 1, 1, 0, 2, 1, 0,
        0, 1, 1, 0, 0, 1, 0,
        0, 2, 2, 0, 0, 2, 0,
        0, 0, 0, 0, 0, 0, 0
    };

    constexpr CharData n_cap {
        7, 9, 8, 6, n_cap_data
        };

    constexpr uint8_t o_cap_data[] = {
        0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 2, 1, 0,
        0, 1, 1, 0, 1, 0,
        0, 1, 1, 0, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 2, 2, 2, 2, 0,
        0, 0, 0, 0, 0, 0,
    };

    constexpr CharData o_cap {
        6, 9, 8, 5, o_cap_data
        };

    constexpr uint8_t p_cap_data[] = {
        0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 2, 1, 0,
        0, 1, 1, 0, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 2, 2, 0,
        0, 1, 1, 0, 0, 0,
        0, 2, 2, 0, 3, 3,
        0, 0, 0, 0, 3, 3,
    };

    constexpr CharData p_cap {
        6, 9, 8, 5, p_cap_data
        };

    constexpr uint8_t q_cap_data[] = {
        0, 0, 0, 0, 0, 0, 3,
        0, 1, 1, 1, 1, 0, 3,
        0, 1, 1, 2, 1, 0, 3,
        0, 1, 1, 0, 1, 0, 3,
        0, 1, 1, 0, 1, 0, 3,
        0, 1, 1, 1, 1, 0, 3,
        0, 1, 1, 1, 1, 0, 0,
        0, 2, 2, 2, 1, 1, 0,
        0, 0, 0, 0, 2, 2, 0,
        3, 3, 3, 0, 0, 0, 0,
    };

    constexpr CharData q_cap {
        7, 10, 8, 6, q_cap_data
        };

    constexpr uint8_t r_cap_data[] = {
        0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 2, 1, 0,
        0, 1, 1, 0, 1, 0,
        0, 1, 1, 1, 2, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 2, 1, 0,
        0, 2, 2, 0, 2, 0,
        0, 0, 0, 0, 0, 0,
    };

    constexpr CharData r_cap {
        6, 9, 8, 5, r_cap_data
        };

    constexpr uint8_t s_cap_data[] = {
        0, 0, 0, 0, 0,
        0, 1, 1, 1, 0,
        0, 1, 1, 2, 0,
        0, 1, 1, 1, 0,
        0, 2, 1, 1, 0,
        0, 0, 1, 1, 0,
        0, 1, 1, 1, 0,
        0, 2, 2, 2, 0,
        0, 0, 0, 0, 0,
    };

    constexpr CharData s_cap {
        5, 9, 8, 4, s_cap_data
        };

    constexpr uint8_t t_cap_data[] = {
        0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 2, 1, 1, 2, 0,
        0, 0, 1, 1, 0, 0,
        3, 0, 1, 1, 0, 3,
        3, 0, 1, 1, 0, 3,
        3, 0, 2, 2, 0, 3,
        3, 0, 0, 0, 0, 3,
    };

    constexpr CharData t_cap {
        6, 9, 8, 5, t_cap_data
        };

    constexpr uint8_t u_cap_data[] = {
        0, 0, 0, 0, 0, 0,
        0, 1, 1, 0, 1, 0,
        0, 1, 1, 0, 1, 0,
        0, 1, 1, 0, 1, 0,
        0, 1, 1, 0, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 2, 2, 2, 2, 0,
        0, 0, 0, 0, 0, 0
    };

    constexpr CharData u_cap {
        6, 9, 8, 5, u_cap_data
        };

    constexpr uint8_t v_cap_data[] = {
        0, 0, 0, 0, 0, 0,
        0, 1, 1, 0, 1, 0,
        0, 1, 1, 0, 1, 0,
        0, 1, 1, 0, 1, 0,
        0, 1, 1, 0, 1, 0,
        0, 2, 1, 1, 2, 0,
        0, 0, 1, 1, 0, 0,
        3, 0, 2, 2, 0, 3,
        3, 0, 0, 0, 0, 3,
    };

    constexpr CharData v_cap {
        6, 9, 8, 5, v_cap_data
        };

    constexpr uint8_t w_cap_data[] = {
        0, 0, 0, 0, 3, 0, 0, 0,
        0, 1, 1, 0, 3, 0, 1, 0,
        0, 1, 1, 0, 0, 0, 1, 0,
        0, 1, 1, 0, 1, 0, 1, 0,
        0, 1, 1, 0, 1, 0, 1, 0,
        0, 2, 1, 1, 1, 1, 1, 0,
        0, 0, 1, 1, 2, 1, 2, 0,
        3, 0, 2, 2, 0, 2, 0, 3,
        3, 0, 0, 0, 0, 0, 0, 3,
    };

    constexpr CharData w_cap {
        8, 9, 8, 7, w_cap_data
        };

    constexpr uint8_t x_cap_data[] = {
        0, 0, 0, 0, 0, 0,
        0, 1, 1, 0, 1, 0,
        0, 1, 1, 0, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 2, 1, 1, 2, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 2, 1, 0,
        0, 2, 2, 0, 2, 0,
        0, 0, 0, 0, 0, 0,
    };

    constexpr CharData x_cap {
        6, 9, 8, 5, x_cap_data
        };

    constexpr uint8_t y_cap_data[] = {
        0, 0, 0, 0, 0, 0,
        0, 1, 1, 0, 1, 0,
        0, 1, 1, 0, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 2, 1, 1, 2, 0,
        0, 0, 1, 1, 0, 0,
        3, 0, 1, 1, 0, 3,
        3, 0, 2, 2, 0, 3,
        3, 0, 0, 0, 0, 3,
    };

    constexpr CharData y_cap {
        6, 9, 8, 5, y_cap_data
        };

    constexpr uint8_t z_cap_data[] = {
        0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 0,
        0, 2, 2, 1, 1, 0,
        0, 0, 1, 1, 2, 0,
        0, 1, 1, 2, 0, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 2, 2, 2, 2, 0,
        0, 0, 0, 0, 0, 0,
    };

    constexpr CharData z_cap {
        6, 9, 8, 5, z_cap_data
        };

    constexpr uint8_t a_low_data[] = {
        0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 2, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 2, 1, 0,
        0, 2, 2, 0, 2, 0,
        0, 0, 0, 0, 0, 0,
    };

    constexpr CharData a_low {
        6, 7, 6, 5, a_low_data
        };

    constexpr uint8_t b_low_data[] = {
        0, 0, 0, 0, 3, 3,
        0, 1, 1, 0, 3, 3,
        0, 1, 1, 0, 0, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 2, 1, 0,
        0, 1, 1, 0, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 2, 2, 2, 2, 0,
        0, 0, 0, 0, 0, 0,
    };

    constexpr CharData b_low {
        6, 9, 8, 5, b_low_data
    };

    constexpr uint8_t c_low_data[] = {
        0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 2, 2, 0,
        0, 1, 1, 0, 0, 0,
        0, 1, 1, 1, 1, 0,
        0, 2, 2, 2, 2, 0,
        0, 0, 0, 0, 0, 0,
    };

    constexpr CharData c_low {
        6, 7, 6, 5, c_low_data
        };

    constexpr uint8_t d_low_data[] = {
        3, 3, 0, 0, 0, 0,
        3, 3, 0, 1, 1, 0,
        0, 0, 0, 1, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 2, 1, 0,
        0, 1, 1, 0, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 2, 2, 2, 2, 0,
        0, 0, 0, 0, 0, 0,
    };

    constexpr CharData d_low {
        6, 9, 8, 5, d_low_data
    };

    constexpr uint8_t e_low_data[] = {
        0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 2, 2, 0,
        0, 1, 1, 1, 1, 0,
        0, 2, 2, 2, 2, 0,
        0, 0, 0, 0, 0, 0,
    };

    constexpr CharData e_low {
        6, 7, 6, 5, e_low_data
        };

    constexpr uint8_t f_low_data[] = {
        0, 0, 0, 0, 0,
        0, 1, 1, 1, 0,
        0, 1, 1, 1, 0,
        0, 1, 1, 2, 0,
        0, 1, 1, 1, 0,
        0, 1, 1, 2, 0,
        0, 1, 1, 0, 0,
        0, 2, 2, 0, 3,
        0, 0, 0, 0, 3,
    };

    constexpr CharData f_low {
        5, 9, 8, 4, f_low_data
        };

    constexpr uint8_t g_low_data[] = {
        0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 2, 1, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 2, 2, 1, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 2, 2, 2, 2, 0,
        0, 0, 0, 0, 0, 0,
    };

    constexpr CharData g_low {
        6, 8, 6, 5, g_low_data
        };

    constexpr uint8_t h_low_data[] = {
        0, 0, 0, 0, 3, 3,
        0, 1, 1, 0, 3, 3,
        0, 1, 1, 0, 0, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 2, 1, 0,
        0, 1, 1, 0, 1, 0,
        0, 2, 2, 0, 2, 0,
        0, 0, 0, 0, 0, 0,
    };

    constexpr CharData h_low {
        6, 9, 8, 5, h_low_data
    };

    constexpr uint8_t i_low_data[] = {
        0, 0, 0, 0,
        0, 1, 1, 0,
        0, 2, 2, 0,
        0, 1, 1, 0,
        0, 1, 1, 0,
        0, 1, 1, 0,
        0, 1, 1, 0,
        0, 2, 2, 0,
        0, 0, 0, 0,
    };

    constexpr CharData i_low {
        4, 9, 8, 3, i_low_data
        };

    constexpr uint8_t j_low_data[] = {
        3, 0, 0, 0, 0,
        3, 0, 1, 1, 0,
        3, 0, 2, 2, 0,
        3, 0, 1, 1, 0,
        3, 0, 1, 1, 0,
        3, 0, 1, 1, 0,
        0, 0, 1, 1, 0,
        0, 1, 1, 1, 0,
        0, 2, 2, 2, 0,
        0, 0, 0, 0, 0,
    };

    constexpr CharData j_low {
        5, 10, 8, 4, j_low_data
        };

    constexpr uint8_t k_low_data[] = {
        0, 0, 0, 0, 3, 3,
        0, 1, 1, 0, 3, 3,
        0, 1, 1, 0, 0, 0,
        0, 1, 1, 0, 1, 0,
        0, 1, 1, 1, 2, 0,
        0, 1, 1, 2, 1, 0,
        0, 1, 1, 0, 1, 0,
        0, 2, 2, 0, 2, 0,
        0, 0, 0, 0, 0, 0,
    };

    constexpr CharData k_low {
        6, 9, 8, 5, k_low_data
    };

    constexpr uint8_t l_low_data[] = {
        0, 0, 0, 0,
        0, 1, 1, 0,
        0, 1, 1, 0,
        0, 1, 1, 0,
        0, 1, 1, 0,
        0, 1, 1, 0,
        0, 1, 1, 0,
        0, 2, 2, 0,
        0, 0, 0, 0,
    };

    constexpr CharData l_low {
        4, 9, 8, 3, l_low_data
        };

    constexpr uint8_t m_low_data[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 1, 1, 0,
        0, 1, 1, 2, 1, 2, 1, 0,
        0, 1, 1, 0, 1, 0, 1, 0,
        0, 1, 1, 0, 2, 0, 1, 0,
        0, 2, 2, 0, 0, 0, 2, 0,
        0, 0, 0, 0, 3, 0, 0, 0,
    };

    constexpr CharData m_low {
        8, 7, 6, 7, m_low_data
    };

    constexpr uint8_t n_low_data[] = {
        0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 2, 1, 0,
        0, 1, 1, 0, 1, 0,
        0, 2, 2, 0, 2, 0,
        0, 0, 0, 0, 0, 0,
    };

    constexpr CharData n_low {
        6, 7, 6, 5, n_low_data
    };

    constexpr uint8_t o_low_data[] = {
        0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 2, 1, 0,
        0, 1, 1, 0, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 2, 2, 2, 2, 0,
        0, 0, 0, 0, 0, 0,
    };

    constexpr CharData o_low {
        6, 7, 6, 5, o_low_data
        };

    constexpr uint8_t p_low_data[] = {
        0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 2, 1, 0,
        0, 1, 1, 0, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 2, 2, 0,
        0, 1, 1, 0, 0, 0,
        0, 2, 2, 0, 3, 3,
        0, 0, 0, 0, 3, 3,
    };

    constexpr CharData p_low {
        6, 9, 6, 5, p_low_data
        };

    constexpr uint8_t q_low_data[] = {
        0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 2, 1, 1, 0,
        0, 1, 0, 1, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 2, 2, 1, 1, 0,
        0, 0, 0, 1, 1, 0,
        3, 3, 0, 2, 2, 0,
        3, 3, 0, 0, 0, 0,
    };

    constexpr CharData q_low {
        6, 9, 6, 5, q_low_data
        };

    constexpr uint8_t r_low_data[] = {
        0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 2, 1, 0,
        0, 1, 1, 0, 2, 0,
        0, 1, 1, 0, 0, 0,
        0, 2, 2, 0, 3, 3,
        0, 0, 0, 0, 3, 3,
    };

    constexpr CharData r_low {
        6, 7, 6, 5, r_low_data
        };

    constexpr uint8_t s_low_data[] = {
        0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 2, 2, 2, 0,
        0, 2, 1, 1, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 2, 2, 2, 2, 0,
        0, 0, 0, 0, 0, 0
    };

    constexpr CharData s_low {
        6, 7, 6, 5, s_low_data
        };

    constexpr uint8_t t_low_data[] = {
        3, 0, 0, 0, 0, 3,
        3, 0, 1, 1, 0, 3,
        0, 0, 1, 1, 0, 0,
        0, 1, 1, 1, 1, 0,
        0, 2, 1, 1, 2, 0,
        0, 0, 1, 1, 0, 0,
        3, 0, 1, 1, 0, 3,
        3, 0, 2, 2, 0, 3,
        3, 0, 0, 0, 0, 3,
    };

    constexpr CharData t_low {
        6, 9, 8, 5, t_low_data
    };

    constexpr uint8_t u_low_data[] = {
        0, 0, 0, 0, 0, 0,
        0, 1, 1, 0, 1, 0,
        0, 1, 1, 0, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 2, 2, 2, 2, 0,
        0, 0, 0, 0, 0, 0,
    };

    constexpr CharData u_low {
        6, 7, 6, 5, u_low_data
        };

    constexpr uint8_t v_low_data[] = {
        0, 0, 0, 0, 0, 0,
        0, 1, 1, 0, 1, 0,
        0, 1, 1, 0, 1, 0,
        0, 1, 1, 0, 1, 0,
        0, 2, 1, 1, 2, 0,
        0, 0, 2, 2, 0, 0,
        3, 0, 0, 0, 0, 3,
    };

    constexpr CharData v_low {
        6, 7, 6, 5, v_low_data
        };

    constexpr uint8_t w_low_data[] = {
        0, 0, 0, 0, 0, 0, 0,
        0, 1, 1, 0, 0, 1, 0,
        0, 1, 1, 0, 1, 1, 0,
        0, 1, 1, 1, 1, 1, 0,
        0, 1, 1, 1, 1, 1, 0,
        0, 2, 2, 2, 2, 2, 0,
        0, 0, 0, 0, 0, 0, 0,
    };

    constexpr CharData w_low {
        7, 7, 6, 6, w_low_data
        };

    constexpr uint8_t x_low_data[] = {
        0, 0, 0, 0, 0, 0,
        0, 1, 1, 0, 1, 0,
        0, 1, 1, 0, 1, 0,
        0, 2, 1, 1, 2, 0,
        0, 1, 1, 2, 1, 0,
        0, 2, 2, 0, 2, 0,
        0, 0, 0, 0, 0, 0,
    };

    constexpr CharData x_low {
        6, 7, 6, 5, x_low_data
        };

    constexpr uint8_t y_low_data[] = {
        0, 0, 0, 0, 0, 0,
        0, 1, 1, 0, 1, 0,
        0, 1, 1, 0, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 2, 2, 2, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 2, 2, 2, 2, 0,
        0, 0, 0, 0, 0, 0,
    };

    constexpr CharData y_low {
        6, 8, 6, 5, y_low_data
        };

    constexpr uint8_t z_low_data[] = {
        0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 0,
        0, 2, 2, 2, 1, 0,
        0, 1, 1, 1, 2, 0,
        0, 1, 1, 1, 1, 0,
        0, 2, 2, 2, 2, 0,
        0, 0, 0, 0, 0, 0,
    };

    constexpr CharData z_low {
        6, 7, 6, 5, z_low_data
        };

    constexpr uint8_t one_data[] = {
        0, 0, 0, 0,
        0, 1, 1, 0,
        0, 1, 1, 0,
        0, 1, 1, 0,
        0, 1, 1, 0,
        0, 2, 2, 0,
        0, 0, 0, 0,
    };

    constexpr CharData one {
    4, 7, 6, 3, one_data
    };

    constexpr uint8_t two_data[] = {
        0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 0,
        0, 2, 2, 1, 1, 0,
        0, 1, 1, 2, 2, 0,
        0, 1, 1, 1, 1, 0,
        0, 2, 2, 2, 2, 0,
        0, 0, 0, 0, 0, 0,
    };

    constexpr CharData two {
    6, 7, 6, 5, two_data
    };

    constexpr uint8_t three_data[] = {
        0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 0,
        0, 2, 1, 1, 1, 0,
        0, 0, 2, 1, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 2, 2, 2, 2, 0,
        0, 0, 0, 0, 0, 0,
    };

    constexpr CharData three {
    6, 7, 6, 5, three_data
    };

    constexpr uint8_t four_data[] = {
        0, 0, 0, 0, 0, 0,
        0, 1, 0, 1, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 2, 2, 1, 1, 0,
        0, 0, 0, 1, 1, 0,
        3, 3, 0, 2, 2, 0,
        3, 3, 0, 0, 0, 0,
    };

    constexpr CharData four {
    6, 7, 6, 5, four_data
    };

    constexpr uint8_t five_data[] = {
        0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 2, 2, 0,
        0, 2, 2, 1, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 2, 2, 2, 2, 0,
        0, 0, 0, 0, 0, 0
    };

    constexpr CharData five {
    6, 7, 6, 5, five_data
    };

    constexpr uint8_t six_data[] = {
        0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 2, 2, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 2, 2, 2, 2, 0,
        0, 0, 0, 0, 0, 0,
    };

    constexpr CharData six {
    6, 7, 6, 5, six_data
    };

    constexpr uint8_t seven_data[] = {
        0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 0,
        0, 2, 2, 1, 1, 0,
        0, 0, 0, 1, 1, 0,
        3, 3, 0, 1, 1, 0,
        3, 3, 0, 2, 2, 0,
        3, 3, 0, 0, 0, 0,
    };

    constexpr CharData seven {
    6, 7, 6, 5, seven_data
    };

    constexpr uint8_t eight_data[] = {
        0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 2, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 2, 2, 2, 2, 0,
        0, 0, 0, 0, 0, 0,
    };

    constexpr CharData eight {
    6, 7, 6, 5, eight_data
    };

    constexpr uint8_t nine_data[] = {
        0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 2, 1, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 2, 2, 1, 1, 0,
        0, 0, 0, 2, 2, 0,
        3, 3, 0, 0, 0, 0,
    };

    constexpr CharData nine {
    6, 7, 6, 5, nine_data
    };

    constexpr uint8_t zero_data[] = {
        0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 1, 2, 1, 0,
        0, 1, 1, 0, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 2, 2, 2, 2, 0,
        0, 0, 0, 0, 0, 0,
    };

    constexpr CharData zero {
    6, 7, 6, 5, zero_data
    };

    constexpr CharData space {
        0, 0, 0, 4, nullptr
    };

    constexpr uint8_t dot_data[] = {
        0, 0, 0, 0,
        0, 1, 1, 0,
        0, 2, 2, 0,
        0, 0, 0, 0,
    };

    constexpr CharData dot {
    4, 4, 3, 3, dot_data
    };

    constexpr uint8_t bang_data[] = {
        0, 0, 0, 0,
        0, 1, 1, 0,
        0, 1, 1, 0,
        0, 1, 1, 0,
        0, 1, 1, 0,
        0, 2, 2, 0,
        0, 1, 1, 0,
        0, 2, 2, 0,
        0, 0, 0, 0,
    };

    constexpr CharData bang {
    4, 9, 8, 3, bang_data
    };

    constexpr uint8_t question_data[] = {
        3, 0, 0, 0, 0, 3,
        0, 0, 1, 1, 0, 0,
        0, 1, 2, 1, 1, 0,
        0, 2, 0, 1, 1, 0,
        0, 0, 1, 1, 2, 0,
        3, 0, 2, 2, 0, 0,
        3, 0, 1, 1, 0, 3,
        3, 0, 2, 2, 0, 3,
        3, 0, 0, 0, 0, 3,
    };

    constexpr CharData question {
    6, 9, 8, 5, question_data
    };

    constexpr uint8_t colon_data[] = {
        0, 0, 0, 0,
        0, 1, 1, 0,
        0, 2, 2, 0,
        0, 0, 0, 0,
        3, 3, 3, 3,
        0, 0, 0, 0,
        0, 1, 1, 0,
        0, 2, 2, 0,
        0, 0, 0, 0,
    };

    constexpr CharData colon {
    4, 9, 8, 3, colon_data
    };

    constexpr uint8_t comma_data[] = {
        3, 0, 0, 0, 0,
        0, 0, 1, 1, 0,
        0, 1, 1, 2, 0,
        0, 2, 2, 0, 0,
        0, 0, 0, 0, 3,
    };

    constexpr CharData comma {
    5, 5, 3, 4, comma_data
    };

    constexpr uint8_t semicolon_data[] = {
        3, 0, 0, 0, 0,
        3, 0, 1, 1, 0,
        3, 0, 2, 2, 0,
        3, 0, 0, 0, 0,
        3, 3, 3, 3, 3,
        3, 0, 0, 0, 0,
        0, 0, 1, 1, 0,
        0, 1, 1, 2, 0,
        0, 2, 2, 0, 0,
        0, 0, 0, 0, 3,
    };

    constexpr CharData semicolon {
    5, 10, 8, 4, semicolon_data
    };

    constexpr uint8_t left_parenthesis_data[] = {
        3, 3, 0, 0, 0, 0,
        3, 0, 0, 1, 1, 0,
        0, 0, 1, 1, 1, 0,
        0, 1, 1, 2, 2, 0,
        0, 1, 1, 0, 0, 0,
        0, 1, 1, 0, 3, 3,
        0, 1, 1, 0, 0, 0,
        0, 2, 1, 1, 1, 0,
        0, 0, 2, 1, 1, 0,
        3, 0, 0, 2, 2, 0,
        3, 3, 0, 0, 0, 0,
    };

    constexpr CharData left_parenthesis {
        6, 11, 9, 5, left_parenthesis_data
    };

    constexpr uint8_t right_parenthesis_data[] = {
        0, 0, 0, 0, 3, 3,
        0, 1, 1, 0, 0, 3,
        0, 1, 1, 1, 0, 0,
        0, 2, 2, 1, 1, 0,
        0, 0, 0, 1, 1, 0,
        3, 3, 0, 1, 1, 0,
        0, 0, 0, 1, 1, 0,
        0, 1, 1, 1, 2, 0,
        0, 1, 1, 2, 0, 0,
        0, 2, 2, 0, 0, 3,
        0, 0, 0, 0, 3, 3,
    };

    constexpr CharData right_parenthesis {
        6, 11, 9, 5, right_parenthesis_data
    };

    constexpr uint8_t left_bracket_data[] = {
        0, 0, 0, 0, 0,
        0, 1, 1, 1, 0,
        0, 1, 1, 2, 0,
        0, 1, 1, 0, 0,
        0, 1, 1, 0, 3,
        0, 1, 1, 0, 3,
        0, 1, 1, 0, 3,
        0, 1, 1, 0, 0,
        0, 1, 1, 1, 0,
        0, 2, 2, 2, 0,
        0, 0, 0, 0, 0,
    };

    constexpr CharData left_bracket {
        5, 11, 9, 4, left_bracket_data
    };

    constexpr uint8_t right_bracket_data[] = {
        0, 0, 0, 0, 0,
        0, 1, 1, 1, 0,
        0, 2, 1, 1, 0,
        0, 0, 1, 1, 0,
        3, 0, 1, 1, 0,
        3, 0, 1, 1, 0,
        3, 0, 1, 1, 0,
        0, 0, 1, 1, 0,
        0, 1, 1, 1, 0,
        0, 2, 2, 2, 0,
        0, 0, 0, 0, 0,
    };

    constexpr CharData right_bracket {
        5, 11, 9, 4, right_bracket_data
    };

    constexpr uint8_t right_brace_data[] = {
        0, 0, 0, 0, 0, 3,
        0, 1, 1, 1, 0, 3,
        0, 2, 1, 1, 0, 3,
        0, 0, 1, 1, 0, 0,
        3, 0, 1, 1, 1, 0,
        3, 0, 1, 1, 1, 0,
        3, 0, 1, 1, 0, 0,
        0, 0, 1, 1, 0, 0,
        0, 1, 1, 1, 0, 3,
        0, 2, 2, 2, 0, 3,
        0, 0, 0, 0, 0, 3,
    };

    constexpr CharData right_brace {
        6, 11, 9, 5, right_brace_data
    };

    constexpr uint8_t left_brace_data[] = {
        3, 0, 0, 0, 0, 0,
        3, 0, 1, 1, 1, 0,
        3, 0, 1, 1, 2, 0,
        0, 0, 1, 1, 0, 0,
        0, 1, 1, 1, 0, 3,
        0, 1, 1, 1, 0, 3,
        0, 0, 1, 1, 0, 3,
        0, 0, 1, 1, 0, 0,
        3, 0, 1, 1, 1, 0,
        3, 0, 2, 2, 2, 0,
        3, 0, 0, 0, 0, 0,
    };

    constexpr CharData left_brace {
        6, 11, 9, 5, left_brace_data
    };

    constexpr uint8_t star_data[] = {
        0, 0, 0, 0, 0,
        0, 1, 0, 1, 0,
        0, 2, 1, 2, 0,
        0, 1, 2, 1, 0,
        0, 2, 0, 2, 0,
        0, 0, 0, 0, 0,
    };

    constexpr CharData star {
        5, 6, 6, 4, star_data
    };

    constexpr uint8_t quotation_data[] = {
        0, 0, 0, 0, 0,
        0, 1, 0, 1, 0,
        0, 1, 0, 1, 0,
        0, 2, 0, 2, 0,
        0, 0, 0, 0, 0,
    };

    constexpr CharData quotation {
    5, 5, 9, 4, quotation_data
    };

    constexpr uint8_t apostrophe_data[] = {
        0, 0, 0,
        0, 1, 0,
        0, 1, 0,
        0, 2, 0,
        0, 0, 0,
    };

    constexpr CharData apostrophe {
    3, 5, 9, 2, apostrophe_data
    };

    constexpr uint8_t backtick_data[] = {
        0, 0, 0, 0, 3,
        0, 1, 1, 0, 0,
        0, 2, 1, 1, 0,
        0, 0, 2, 2, 0,
        3, 0, 0, 0, 0,
    };

    constexpr CharData backtick {
        5, 5, 8, 4, backtick_data
    };

    constexpr uint8_t tilde_data[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 0, 1, 0,
        0, 1, 1, 2, 1, 1, 1, 0,
        0, 2, 2, 0, 2, 2, 2, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
    };

    constexpr CharData tilde {
    8, 5, 6, 7, tilde_data
    };

    constexpr uint8_t underscore_data[] = {
        0, 0, 0, 0, 0,
        0, 1, 1, 1, 0,
        0, 2, 2, 2, 0,
        0, 0, 0, 0, 0,
    };

    constexpr CharData underscore {
    5, 4, 2, 4, underscore_data
    };

    constexpr uint8_t dollar_data[] = {
        3, 0, 0, 0, 0, 3,
        0, 0, 1, 1, 0, 0,
        0, 1, 1, 1, 1, 0,
        0, 1, 2, 2, 2, 0,
        0, 2, 1, 1, 1, 0,
        0, 1, 1, 1, 1, 0,
        0, 2, 1, 1, 2, 0,
        0, 0, 2, 2, 0, 0,
        3, 0, 0, 0, 0, 3,
    };

    constexpr CharData dollar {
    6, 9, 8, 5, dollar_data
    };

    inline CharData char_to_data(const char c) {
        switch (c) {
            case 'A': return a_cap;
            case 'B': return b_cap;
            case 'C': return c_cap;
            case 'D': return d_cap;
            case 'E': return e_cap;
            case 'F': return f_cap;
            case 'G': return g_cap;
            case 'H': return h_cap;
            case 'I': return i_cap;
            case 'J': return j_cap;
            case 'K': return k_cap;
            case 'L': return l_cap;
            case 'M': return m_cap;
            case 'N': return n_cap;
            case 'O': return o_cap;
            case 'P': return p_cap;
            case 'Q': return q_cap;
            case 'R': return r_cap;
            case 'S': return s_cap;
            case 'T': return t_cap;
            case 'U': return u_cap;
            case 'V': return v_cap;
            case 'W': return w_cap;
            case 'X': return x_cap;
            case 'Y': return y_cap;
            case 'Z': return z_cap;
            case 'a': return a_low;
            case 'b': return b_low;
            case 'c': return c_low;
            case 'd': return d_low;
            case 'e': return e_low;
            case 'f': return f_low;
            case 'g': return g_low;
            case 'h': return h_low;
            case 'i': return i_low;
            case 'j': return j_low;
            case 'k': return k_low;
            case 'l': return l_low;
            case 'm': return m_low;
            case 'n': return n_low;
            case 'o': return o_low;
            case 'p': return p_low;
            case 'q': return q_low;
            case 'r': return r_low;
            case 's': return s_low;
            case 't': return t_low;
            case 'u': return u_low;
            case 'v': return v_low;
            case 'w': return w_low;
            case 'x': return x_low;
            case 'y': return y_low;
            case 'z': return z_low;
            case ' ': return space;
            case '0': return zero;
            case '1': return one;
            case '2': return two;
            case '3': return three;
            case '4': return four;
            case '5': return five;
            case '6': return six;
            case '7': return seven;
            case '8': return eight;
            case '9': return nine;
            case '.': return dot;
            case '!': return bang;
            case '?': return question;
            case ',': return comma;
            case ':': return colon;
            case ';': return semicolon;
            case '*': return star;
            case '(': return left_parenthesis;
            case ')': return right_parenthesis;
            case '[': return left_bracket;
            case ']': return right_bracket;
            case '{': return left_brace;
            case '}': return right_brace;
            case '\'': return apostrophe;
            case '\"': return quotation;
            case '`': return backtick;
            case '~': return tilde;
            case '_': return underscore;
            case '$': return dollar;
            default: return space;
        }
    }
}

#endif //GRAPHITE_FONT_OMNI_H

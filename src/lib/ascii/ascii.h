
#pragma once
// orginal code
//https://github.com/NathanKolpa/ASCII-Enum/blob/master/include/ascii-table.h
/*
* Made by Nathan Kolpa
*/

enum AsciiTable
{
  //control characters

	ASCII_NULL,                                            // NULL
	ASCII_SOH,                                             // Start of Heading
	ASCII_STX,                                             // Start of Text
	ASCII_ETX,                                             // End of Text
	ASCII_EOT,                                             // End of Transmission
	ASCII_ENQ,                                             // Enquiry
	ASCII_ACK,                                             // Acknowledgement
	ASCII_BEL,                                             // Bell
	ASCII_BS,                                              // Backspace
	ASCII_TAB,                                             // Horizontal Tab
	ASCII_LF,                                              // Line Feed 
	ASCII_VT,                                              // Vertical Tab 
	ASCII_FF,                                              // Form Feed 
	ASCII_CR,                                              // Carriage Return
	ASCII_SO,                                              // Shift Out 
	ASCII_SI,                                              // Shift In 
	ASCII_DLE,                                             // Data Link Escape 
	ASCII_DC1,                                             // Device Control 1
	ASCII_DC2,                                             // Device Control 2
	ASCII_DC3,                                             // Device Control 3
	ASCII_DC4,                                             // Device Control 4
	ASCII_NAK,                                             // Negative Acknowledgement 
	ASCII_SYN,                                             // Synchronous Idle 
	ASCII_ETB,                                             // End of Transmission Block 
	ASCII_CAN,                                             // Cancel
	ASCII_EM,                                              // End of Medium 
	ASCII_SUB,                                             // Substitute 
	ASCII_ESC,                                             // Escape
	ASCII_SF,                                              // File Separator
	ASCII_GS,                                              // Group Separator
	ASCII_RS,                                              // Record Separator
	ASCII_US,                                              // Unit Separator

	//misc characters				                 

	ASCII_SPACE,                                           // space
	ASCII_EXCLAMAION_MARK,                                 // !
	ASCII_QUOTATION_MARK_DOUBLE,                           // "
	ASCII_HASHTAG,                                         // #
	ASCII_SING_DOLLAR,                                     // $
	ASCII_PRECENT,                                         // %
	ASCII_AMPERSANT,                                       // &
	ASCII_QUOTATION_MARK_SINGLE,                           // '
	ASCII_PARENTHESES_OPEN,                                // (
	ASCII_PARENTHESES_CLOSE,                               // )
	ASCII_ASTERISK,                                        // *
	ASCII_PLUS,                                            // +
	ASCII_COMMA,                                           // ,
	ASCII_MINUS,                                           // -
	ASCII_DOT,                                             // .
	ASCII_SLASH_FOWARD,                                    // /
	ASCII_NUMBER_0,                                        // 0
	ASCII_NUMBER_1,                                        // 1
	ASCII_NUMBER_2,                                        // 2
	ASCII_NUMBER_3,                                        // 3
	ASCII_NUMBER_4,                                        // 4
	ASCII_NUMBER_5,                                        // 5
	ASCII_NUMBER_6,                                        // 6
	ASCII_NUMBER_7,                                        // 7
	ASCII_NUMBER_8,                                        // 8
	ASCII_NUMBER_9,                                        // 9
	ASCII_COLON,                                           // :
	ASCII_COLON_SEMI,                                      // ;
	ASCII_LESS_THAN,                                       // <
	ASCII_EQUAL_TO,                                        // =
	ASCII_GREATER_THAN,                                    // >
	ASCII_QUESTION_MARK,                                   // ?
	ASCII_SING_AT,                                         // @

	//upper case alphabet			                 

	ASCII_A_UPPER,                                         // A
	ASCII_B_UPPER,                                         // B
	ASCII_C_UPPER,                                         // C
	ASCII_D_UPPER,                                         // D
	ASCII_E_UPPER,                                         // E
	ASCII_F_UPPER,                                         // F
	ASCII_G_UPPER,                                         // G
	ASCII_H_UPPER,                                         // H
	ASCII_I_UPPER,                                         // I
	ASCII_J_UPPER,                                         // J
	ASCII_K_UPPER,                                         // K
	ASCII_L_UPPER,                                         // L
	ASCII_M_UPPER,                                         // M
	ASCII_N_UPPER,                                         // N
	ASCII_O_UPPER,                                         // O
	ASCII_P_UPPER,                                         // P
	ASCII_Q_UPPER,                                         // Q
	ASCII_R_UPPER,                                         // R
	ASCII_S_UPPER,                                         // S
	ASCII_T_UPPER,                                         // T
	ASCII_U_UPPER,                                         // U
	ASCII_V_UPPER,                                         // V
	ASCII_W_UPPER,                                         // W
	ASCII_X_UPPER,                                         // X
	ASCII_Y_UPPER,                                         // Y
	ASCII_Z_UPPER,                                         // Z

	//misc characters				                 

	ASCII_BRACKET_OPEN,                                    // [
	ASCII_SLASH_BACKWARD,                                  // 
	ASCII_BRACKET_CLOSE,                                   // ]
	ASCII_CARET,                                           // ^
	ASCII_UNDERSCORE,                                      // _
	ASCII_GRAVE_ACCENT,                                    // ` 

	//lower case alphabet			                 

	ASCII_A_LOWER,                                         // a
	ASCII_B_LOWER,                                         // b
	ASCII_C_LOWER,                                         // c
	ASCII_D_LOWER,                                         // d
	ASCII_E_LOWER,                                         // e
	ASCII_F_LOWER,                                         // f
	ASCII_G_LOWER,                                         // g
	ASCII_H_LOWER,                                         // h
	ASCII_I_LOWER,                                         // i
	ASCII_J_LOWER,                                         // j
	ASCII_K_LOWER,                                         // k
	ASCII_L_LOWER,                                         // l
	ASCII_M_LOWER,                                         // m
	ASCII_N_LOWER,                                         // n
	ASCII_O_LOWER,                                         // o
	ASCII_P_LOWER,                                         // p
	ASCII_Q_LOWER,                                         // q
	ASCII_R_LOWER,                                         // r
	ASCII_S_LOWER,                                         // s
	ASCII_T_LOWER,                                         // t
	ASCII_U_LOWER,                                         // u
	ASCII_V_LOWER,                                         // v
	ASCII_W_LOWER,                                         // w
	ASCII_X_LOWER,                                         // x
	ASCII_Y_LOWER,                                         // y
	ASCII_Z_LOWER,                                         // z

	//misc characters				                 

	ASCII_BRACKET_CURLY_OPEN,                              // {
	ASCII_VERTICAL_BAR,                                    // |
	ASCII_BRACKET_CURLY_CLOSE,                             // }
	ASCII_TILDE,                                           // ~

	ASCII_DEL,                                             // Delete

	//===================[extended ASCII]===================

	//misc letters					                 

	ASCII_C_UPPER_CEDILLA,                                 // Ç

	ASCII_U_LOWER_UMLAUT,                                  // ü
	ASCII_E_LOWER_ACUTE,                                   // é 
	ASCII_A_LOWER_CIRCUMFLEX,                              // â
	ASCII_A_LOWER_UMLAUT,                                  // ä
	ASCII_A_LOWER_GRAVE,                                   // à
	ASCII_A_LOWER_RING,                                    // å
	ASCII_C_LOWER_CEDILLA,                                 // ç
	ASCII_E_LOWER_CIRCUMFLEX,                              // ê
	ASCII_E_LOWER_UMLAUT,                                  // ë 
	ASCII_E_LOWER_GRAVE,                                   // è
	ASCII_I_LOWER_UMLAUT,                                  // ï 
	ASCII_I_LOWER_CIRCUMFLEX,                              // î
	ASCII_I_LOWER_GRAVE,                                   // ì
	ASCII_A_UPPER_UMLAUT,                                  // Ä
	ASCII_A_UPPER_RING,                                    // Å
	ASCII_E_UPPER_ACUTE,                                   // É
	ASCII_AE_LOWER,                                        // æ 
	ASCII_AE_UPPER,                                        // Æ 
	ASCII_O_LOWER_CIRCUMFLEX,                              // ô
	ASCII_O_LOWER_UMLAUT,                                  // ö
	ASCII_O_LOWER_GRAVE,                                   // ò
	ASCII_U_LOWER_CIRCUMFLEX,                              // û
	ASCII_U_LOWER_GRAVE,                                   // ù
	ASCII_Y_LOWER_UMLAUT,                                  // ÿ
	ASCII_O_UPPER_UMLAUT,                                  // Ö
	ASCII_U_UPPER_UMLAUT,                                  // Ü

	ASCII_SING_CENT,                                       // ¢
	ASCII_SING_POUND,                                      // £
	ASCII_SING_YEN,                                        // ¥
	ASCII_SING_PESTA,                                      // ₧
	ASCII_F_LOWER_HOOK,                                    // ƒ

	ASCII_A_LOWER_ACUTE,                                   // á
	ASCII_I_LOWER_ACUTE,                                   // í
	ASCII_O_LOWER_ACUTE,                                   // ó
	ASCII_U_LOWER_ACUTE,                                   // ú
	ASCII_N_LOWER_TILDE,                                   // ñ
	ASCII_N_UPPER_TILDE,                                   // Ñ

	//symbols						                 

	ASCII_ORDINAL_INDICATOR_FEMININE,                      // ª
	ASCII_ORDINAL_INDICATOR_MASCULINE,                     // º
	ASCII_QUESTION_MARK_REVERSED,                          // ¿
	ASCII_SING_NOT_REVERSED,                               // ⌐
	ASCII_SING_NOT,                                        // ¬
	ASCII_VULGAR_FRACTION_HALF,                            // ½
	ASCII_VULGAR_FRACTION_QUARTER,                         // ¼
	ASCII_EXCLAMATION_MARK_INVERTED,                       // ¡
	ASCII_QUOTATION_MARK_DOUBLE_ANGLE_LEFT,                // «
	ASCII_QUOTATION_MARK_DOUBLE_ANGLE_RIGHT,               // »

	/* box drawings
	* BOX_[direction1]_[thickness]_[direction2]_[thickness]
	* if you cant find a combination try switching the direction combination
	*/

	ASCII_SHADE_LIGHT,                                     // ░
	ASCII_SHADE_MEDIUM,                                    // ▒
	ASCII_SHADE_DARK,                                      // ▓

	ASCII_BOX_VERTICAL_LIGHT,                              // │
	ASCII_BOX_VERTICAL_LIGHT_LEFT_LIGHT,                   // ┤
	ASCII_BOX_VERTICAL_SINGLE_LEFT_DOUBLE,                 // ╡
	ASCII_BOX_VERTICAL_DOUBLE_LEFT_SINGLE,                 // ╢
	ASCII_BOX_DOWN_DOUBLE_LEFT_SINGLE,                     // ╖
	ASCII_BOX_DOWN_SINGLE_LEFT_DOUBLE,                     // ╕
	ASCII_BOX_VERTICAL_DOUBLE_LEFT_DOUBLE,                 // ╣
	ASCII_BOX_VERTICAL_DOUBLE,                             // ║
	ASCII_BOX_DOWN_DOUBLE_LEFT_DOUBLE,                     // ╗
	ASCII_BOX_UP_DOUBLE_LEFT_DOUBLE,                       // ╝
	ASCII_BOX_UP_DOUBLE_LEFT_SINGLE,                       // ╜
	ASCII_BOX_UP_SINGLE_LEFT_DOUBLE,                       // ╛
	ASCII_BOX_DOWN_LIGHT_LEFT_LIGHT,                       // ┐
	ASCII_BOX_UP_LIGHT_RIGHT_LIGHT,                        // └
	ASCII_BOX_UP_LIGHT_HORIZONTAL_LIGHT,                   // ┴
	ASCII_BOX_DOWN_LIGHT_HORIZONTAL_LIGHT,                 // ┬
	ASCII_BOX_VERTICAL_LIGHT_RIGHT_LIGHT,                  // ├
	ASCII_BOX_HORIZONTAL_LIGHT,                            // ─
	ASCII_BOX_VERTICAL_LIGHT_HORIZONTAL_LIGHT,             // ┼
	ASCII_BOX_VERTICAL_SINGLE_RIGHT_DOUBLE,                // ╞
	ASCII_BOX_VERTICAL_DOUBLE_RIGHT_SINGLE,                // ╟
	ASCII_BOX_UP_DOUBLE_RIGHT_DOUBLE,                      // ╚
	ASCII_BOX_DOWN_DOUBLE_RIGHT_DOUBLE,                    // ╔
	ASCII_BOX_UP_DOUBLE_HORIZONTAL_DOUBLE,                 // ╩
	ASCII_BOX_DOWN_DOUBLE_HORIZONTAL_DOUBLE,               // ╦
	ASCII_BOX_VERTICAL_DOUBLE_RIGHT_DOUBLE,                // ╠
	ASCII_BOX_HORIZONTAL_DOUBLE,                           // ═
	ASCII_BOX_VERTICAL_DOUBLE_HORIZONTAL_DOUBLE,           // ╬
	ASCII_BOX_UP_SINGLE_HORIZONTAL_DOUBLE,                 // ╧
	ASCII_BOX_UP_DOUBLE_HORIZONTAL_SINGLE,                 // ╨
	ASCII_BOX_DOWN_SINGLE_HORIZONTAL_DOUBLE,               // ╤
	ASCII_BOX_DOWN_DOUBLE_HORIZONTAL_SINGLE,               // ╥
	ASCII_BOX_UP_DOUBLE_RIGHT_SINGLE,                      // ╙
	ASCII_BOX_UP_SINGLE_RIGHT_SINGLE,                      // ╘
	ASCII_BOX_DOWN_SINGLE_RIGHT_DOUBLE,                    // ╒
	ASCII_BOX_DOWN_SINGLE_RIGHT_SINGLE,                    // ╓
	ASCII_BOX_VETRICAL_DOUBLE_HORIZONTAL_SINGLE,           // ╫
	ASCII_BOX_VERTICAL_SINGLE_HORIZONTALDOUBLE,            // ╪
	ASCII_BOX_UP_LIGHT_LEFT_LIGHT,                         // ┘
	ASCII_BOX_DOWN_LIGHT_RIGHT_LIGHT,                      // ┌

	ASCII_BLOCK_FULL,                                      // █
	ASCII_BLOCK_HALF_LOWER,                                // ▄
	ASCII_BLOCK_HALF_LEFT,                                 // ▌
	ASCII_BLOCK_HALF_RIGHT,                                // ▐
	ASCII_BLOCK_HALF_UPPER,                                // ▀

	//other letters

	ASCII_APLHA_LOWER,                                     // α
	ASCII_S_SHARP_LOWER,                                   // ß
	ASCII_GAMMA_UPPER,                                     // Γ
	ASCII_PI_LOWER,                                        // π
	ASCII_SIGMA_UPPER,                                     // Σ
	ASCII_SIGMA_LOWER,                                     // σ

	ASCII_SING_MICRO,                                      // µ
	ASCII_TAU_LOWER,                                       // τ
	ASCII_PHI_UPPER,                                       // Φ
	ASCII_THETA_UPPER,                                     // Θ
	ASCII_OMEGA_UPPER,                                     // Ω
	ASCII_DELTA_LOWER,                                     // δ
	ASCII_INFINITY,                                        // ∞
	ASCII_PHI_LOWER,                                       // φ
	ASCII_EPSILON_LOWER,                                   // ε
	ASCII_INTERSECTION,                                    // ∩
	ASCII_IDENTICAL_TO,                                    // ≡
	ASCII_SING_PLUS_MINUS,                                 // ±
	ASCII_GREATER_THAN_OR_EQUAL_TO,                        // ≥
	ASCII_LESS_THAN_OR_EQUAL_TO,                           // ≤
	ASCII_HALF_INTEGRAL_TOP,                               // ⌠
	ASCII_HALF_INTEGRAL_BOTTOM,                            // ⌡
	ASCII_SING_DIVISION,                                   // ÷
	ASCII_EQUAL_TO_ALMOST,                                 // ≈
	ASCII_SING_DEGREE,                                     // °
	ASCII_BULLER_OPERATOR,                                 // ∙
	ASCII_DOT_MIDDLE,                                      // ·
	ASCII_SQUARE_ROOT,                                     // √
	ASCII_N_SUPERSCRIPT_LOWER,                             // ⁿ
	ASCII_NUMBER_2_SUPERSCRIPT,                            // ²
	ASCII_BLACK_SQUARE,                                    // ■
	ASCII_SPACE_NO_BREAK,                                  //
};

/*
 * Copyright 2021 merkrafter
 *
 * This file is part of libsanjego.
 *
 * libsanjego is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libsanjego is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libsanjego. If not, see <https://www.gnu.org/licenses/>.
 */
#pragma once

#include <cstdint>
#include <limits>

namespace libsanjego {
/*
 * Maximum supported type to represent the board dimensions.
 **/
typedef std::uint8_t board_size_t;

/*
 * Maximum supported type to represent the tower height.
 * Is always greater than the board size type.
 */
typedef std::uint16_t tower_size_t;

/*
 * Type to represent dimensions and indices of a board row.
 */
struct RowNr {
  constexpr RowNr(board_size_t v) noexcept : m_value(v){};
  constexpr operator board_size_t() const noexcept { return m_value; }
  const board_size_t m_value;
};

/*
 * Type to represent dimensions and indices of a board column.
 */
struct ColumnNr {
  constexpr ColumnNr(board_size_t v) noexcept : m_value(v){};
  constexpr operator board_size_t() const noexcept { return m_value; }
  const board_size_t m_value;
};

}  // namespace libsanjego
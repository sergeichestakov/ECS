package eval

import (
  "fmt"
)

type UnitKind uint
const (
  Scalar = iota
  Length
  Time
  Mass
  Temperature
  Volume
  Speed
  Fuel
)

type Unit struct {
  kind UnitKind
  scale float64
  offset float64
}

var units = map[string]Unit{
  "scalar": {Scalar, 1, 0},

  // Length
  "m": {Length, 1, 0},
  "km": {Length, 1000, 0},
  "mi": {Length, 1609.344, 0},

  // Time
  "s": {Time, 1, 0},
  "ms": {Time, 0.001, 0},
  "min": {Time, 60, 0},

  // Mass
  "kg": {Mass, 1, 0},
  "g": {Mass, 0.001, 0},
  "lbs": {Mass, 0.45359237, 0},

  // Temperature
  "K": {Temperature, 1, 0},
  "C": {Temperature, 1, 273.15},
  "F": {Temperature, 5.0/9.0, 459.67},
  // temperature: C  F  kelvin

  //Volume
  "ltr": {Volume, 1, 0},
  "gal": {Volume, 3.78541, 0},
}


func (l Literal) FlattenUnits() (Expr, string) {
  return l, "scalar"
}

func (v Var) FlattenUnits() (Expr, string) {
  return v, "scalar"
}

func (u unary) FlattenUnits() (Expr, string) {
  x, x_unit := u.x.FlattenUnits()
  return unary{u.op, x}, x_unit
}

func (b binary) FlattenUnits() (Expr, string) {
  x, x_unit := b.x.FlattenUnits()
  y, y_unit := measure{x_unit, b.y}.FlattenUnits()

  switch b.op {
  case '+', '-':
    return binary{b.op, x, y}, x_unit
  default:
    if units[x_unit].kind != Scalar && units[y_unit].kind != Scalar {
        if b.op == '*' {
            panic("Invalid Operator")
        } else {
            valid_unit := units[y_unit].kind == Time || units[y_unit].kind == Volume
            if units[x_unit].kind == Length && valid_unit {
                final_unit := x_unit + "_p_" + y_unit
                return binary{'/', x, y}, final_unit
            }
        }
    }
    return binary{b.op, x, y}, x_unit
  }
}

func (m measure) FlattenUnits() (Expr, string) {
  x, x_unit := m.x.FlattenUnits()

  m_scale := units[m.unit]
  x_scale := units[x_unit]

  if x_unit == "scalar" {
    return x, m.unit
  } else if x_scale.kind != m_scale.kind {
    if m_scale.kind == Length && (x_scale.kind == Time || x_scale.kind == Volume) {
        return x, x_unit
    } else {
        panic(fmt.Sprintf("Can't convert between units of different kinds"))
    }
  } else if x_scale.scale == m_scale.scale && x_scale.offset == m_scale.offset {
    return x, m.unit
  } else if x_scale.scale == m_scale.scale {
    return binary{'+', x, Literal(x_scale.offset - m_scale.offset)}, m.unit
  } else {
    return binary{'+',
      binary{'*', x, Literal(x_scale.scale/m_scale.scale)},
      Literal(x_scale.offset*x_scale.scale/m_scale.scale - m_scale.offset),
    }, m.unit
  }
}

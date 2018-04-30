package eval

import (
  "fmt"
  "testing"
)

//!+FlattenUnits
func TestFlattenUnits(t *testing.T) {
  tests := []struct {
    expr string
    want_mag string
    want_unit string
  } {
    {"10", "10", "scalar"},
    {"X", "42", "scalar"},
    {"-X", "-42", "scalar"},
    {"5 * 2", "10", "scalar"},
    {"C(K(0))", "-273.15", "C"},
    {"km(10)", "10", "km"},
    {"km(km(10))", "10", "km"},
    {"m(km(10))", "10000", "m"},
    {"F(C(0))", "32", "F"},
    {"s(4) + min(5)", "304", "s"},
    {"F(1) + K(25)", "-413.67", "F"},
    {"K(25) + F(1)", "280.928", "K"},
    {"m(10) / s(2)", "5", "m_p_s"},
    {"mi(40) / gal(2)", "20", "mi_p_gal"},
    {"ltr(gal(1))", "3.78541", "ltr"},
  }

  for _, test := range tests {
    expr, err := Parse(test.expr)
    if err != nil {
      t.Error(err) // parse error
      continue
    }

    // Run the method
    expr, unit := expr.FlattenUnits()

    // Display the result
    got := fmt.Sprintf("%.6g", expr.Eval(Env{"X":42}))
    fmt.Printf("\n%s\n", test.expr)
    fmt.Printf("\t%s => %s [%s]\n", expr, got, unit)

    // Check the result
    if got != test.want_mag || unit != test.want_unit {
      t.Errorf("(%s).FlattenUnits() = %q [%q], want %q [%q]\n",
        test.expr, got, unit, test.want_mag, test.want_unit)
    }
  }
}

func TestFlattenUnits_Failure(t *testing.T) {
  tests := []struct {
    expr string
  } {
    {"m(10) + F(1)"},
    {"m(10) * m(1)"},
    {"m_p_s(5) / s(1)"},
  }

  for _, test := range tests {
    expr, err := Parse(test.expr)
    if err != nil {
      t.Error(err) // parse error
      continue
    }

    func() {
      defer func() {
        if recover() == nil {
          t.Errorf("(%s).FlattenUnits() did not panic, but should\n",
            test.expr)
        }
      }()

      expr.FlattenUnits()
    }()
  }
}
//!-FlattenUnits

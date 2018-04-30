package eval

import (
  "fmt"
  "testing"
)

//!+Simplify
func TestSimplify(t *testing.T) {
  tests := []struct {
    expr string
    env  Env
    want string
  } {
    {"10 / X", Env{"X": 2}, "5"},
    {"(X + X) - Y", Env{"X": 2}, "(4 - Y)"},
    {"(X + X) - Y", Env{"Y": 8}, "((X + X) - 8)"},
    {"5 + 2", Env{}, "7"},
    {"10 - 1 + X - Y", Env{}, "((9 + X) - Y)"},
    {"X + 3 + 5", Env{}, "((X + 3) + 5)"},
    {"+Y", Env{"Y": 4}, "4"},
    {"-K", Env{"K": 20}, "-20"},
    {"-B", Env{}, "(-B)"},
    {"0 + X", Env{}, "X"},
    {"N + 0", Env{}, "N"},
    {"G * 0", Env{}, "0"},
    {"0 * Q", Env{}, "0"},
    {"L * 1", Env{}, "L"},
    {"1 * P", Env{}, "P"},
  }

  for _, test := range tests {
    expr, err := Parse(test.expr)
    if err != nil {
      t.Error(err) // parse error
      continue
    }

    // Run the method
    result := expr.Simplify(test.env)

    // Display the result
    got := Format(result)
    fmt.Printf("\n%s\n", test.expr)
    fmt.Printf("\t%v => %s\n", test.env, got)

    // Check the result
    if got != test.want {
      t.Errorf("(%s).Simplify() in %v = %q, want %q\n",
        test.expr, test.env, got, test.want)
    }
  }
}

func TestSimplify_Failure(t *testing.T) {
  tests := []struct {
    expr Expr
  } {
    {measure{"m", Literal(10.0)}},
  }

  for _, test := range tests {
    func() {
      defer func() {
        if recover() == nil {
          t.Errorf("(%s).Simplify(Env{}) did not panic, but should\n",
            test.expr)
        }
      }()

      test.expr.Simplify(Env{})
    }()
  }
}
//!-Simplify

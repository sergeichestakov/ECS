package analysis

import (
  "go/ast"
  "go/parser"
  "go/token"
  "go/format"
  "bytes"
  "eval"
  "strconv"
)

// rewriteCalls should modify the passed AST
func rewriteCalls(f *ast.File) {
    ast.Inspect(f, func (node ast.Node) bool {
        // We only want to operate on call expressions,
        // so let's filter for those kinds of AST node.
        switch v := node.(type) {
        //Search through function calls
        case *ast.CallExpr:
            switch fun := v.Fun.(type) {
            case *ast.SelectorExpr:
                caller := fun.X.(*ast.Ident).Name
                function := fun.Sel.Name
                //Find correct function
                if caller == "eval" && function == "ParseAndEval" {
                    //Modify first param
                    switch first_param := v.Args[0].(type) {
                    case *ast.BasicLit:
                        if len(v.Args) == 2 { //Must have 2 args
                            //Clean input
                            paramStr, parseErr := strconv.Unquote(first_param.Value)
                            if parseErr == nil {
                                //Parse, Format, and Eval
                                expr, err := eval.Parse(paramStr)
                                if err == nil {
                                    result := expr.Simplify(eval.Env{})
                                    num := result.Eval(eval.Env{})
                                    str := strconv.FormatFloat(num, 'f', -1, 64)
                                    str = strconv.Quote(str)
                                    //Create new node and append
                                    newNode := &ast.BasicLit{Kind: token.STRING, Value: str}
                                    v.Args[0] = newNode
                                }
                            }
                            
                        }
                    }
                }

            }
        }

        return true
    })
}

func SimplifyParseAndEval(src string) string {
  fset := token.NewFileSet()
  f, err := parser.ParseFile(fset, "src.go", src, 0)
  if err != nil {
    panic(err)
  }
  
  //ast.Print(fset, f)
  rewriteCalls(f)

  var buf bytes.Buffer
  format.Node(&buf, fset, f)
  return buf.String()
}

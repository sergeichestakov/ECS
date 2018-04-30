package analysis

import (
  "go/ast"
  "go/parser"
  "go/token"
)

func branchCount(fn *ast.FuncDecl) uint {
    var count uint = 0
    ast.Inspect(fn.Body, func(node ast.Node) bool {
        switch node.(type) {
        case *ast.IfStmt, *ast.ForStmt, *ast.SwitchStmt, *ast.RangeStmt:
            count += 1
        }
        return true
     })
    return count
}

func ComputeBranchFactors(src string) map[string]uint {
  fset := token.NewFileSet()
  f, err := parser.ParseFile(fset, "src.go", src, 0)
  if err != nil {
    panic(err)
  }

  m := make(map[string]uint)
  for _, decl := range f.Decls {
    switch fn := decl.(type) {
    case *ast.FuncDecl:
      m[fn.Name.Name] = branchCount(fn)
    }
  }

  return m
}

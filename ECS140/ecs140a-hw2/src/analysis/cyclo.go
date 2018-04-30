package analysis

import (
  "go/ast"
  "go/parser"
  "go/token"
)

func cyclomatic(node ast.Stmt) uint {
    var count uint = 0
    switch node.(type) {
    case *ast.BlockStmt:
        ret := node.(*ast.BlockStmt)
        count += 1
        for _, stmt := range ret.List {
           count *= cyclomatic(stmt) 
        }
        return count
    case *ast.CaseClause:
        count += 1
        ret := node.(*ast.CaseClause)
        for _, stmt := range ret.Body {
           count *= cyclomatic(stmt) 
        }
        return count
    case *ast.SwitchStmt:
        ret := node.(*ast.SwitchStmt)
        for _, clause := range ret.Body.List {
            count += cyclomatic(clause) 
        }
        return count
    case *ast.TypeSwitchStmt:
        ret := node.(*ast.TypeSwitchStmt)
        for _, clause := range ret.Body.List {
            count += cyclomatic(clause) 
        }
        return count
    case *ast.ForStmt:
        count += 1
        ret := node.(*ast.ForStmt)
        count += cyclomatic(ret.Body)
        return count
    case *ast.RangeStmt:
        count += 1
        ret := node.(*ast.RangeStmt)
        count += cyclomatic(ret.Body)
        return count
    case *ast.IfStmt:
        ret := node.(*ast.IfStmt)
        count += cyclomatic(ret.Body)
        if ret.Else != nil {
            count += cyclomatic(ret.Else)
        }
        return count
    default:
        return 1
    }
}

func CyclomaticComplexity(src string) map[string]uint {
  fset := token.NewFileSet()
  f, err := parser.ParseFile(fset, "src.go", src, 0)
  if err != nil {
    panic(err)
  }

  m := make(map[string]uint)
  for _, decl := range f.Decls {
    switch fn := decl.(type) {
    case *ast.FuncDecl:
      m[fn.Name.Name] = cyclomatic(fn.Body)
    }
  }

  return m
}

package eval

import "reflect"

func (v Var) Simplify(env Env) Expr {
    if val,ok := env[v]; ok {
        return Literal(val) 
    }
    return v
}

func (f Literal) Simplify(env Env) Expr {
    return f
}

func (u unary) Simplify(env Env) Expr {
    u.x = u.x.Simplify(env)
    if reflect.TypeOf(u.x) == reflect.TypeOf(Literal(0)) {
        switch u.op {
        case '+':
            return u.x 
        case '-':
            return Literal(-1 * u.x.Eval(env))  
        }
    } 
    return u
}

func (b binary) Simplify(env Env) Expr {
    //Simplify first
    x := b.x.Simplify(env)
    y := b.y.Simplify(env)
    //Check if x and y are literals
    xType := reflect.TypeOf(x)
    yType := reflect.TypeOf(y)
    litType := reflect.TypeOf(Literal(0))
    //Both Literals
    if xType == litType && yType == litType { 
        return Literal(b.Eval(env))
    } //One Literal
    if xType == litType {
        if x.Eval(env) == 0 {
            switch b.op {
            case '+':
                return y 
            case '*':
                return Literal(0)
            }
        } else if x.Eval(env) == 1 && b.op == '*' {
            return y 
        }
    }
    if yType == litType {
        if y.Eval(env) == 0 {
            switch b.op {
            case '+':
                return x 
            case '*':
                return Literal(0) 
            }
        } else if y.Eval(env) == 1 && b.op == '*' {
            return x 
        }    
    }
    //Modify and return
    b.x = x
    b.y = y
    return b 
}

func (m measure) Simplify(env Env) Expr {
  // don't need to implement this
  panic("cannot simplify measure expression")
}

package eval

func (v Var) Depth() uint {
    return 1
}

func (f Literal) Depth() uint {
    return 1
}

func (u unary) Depth() uint {
    return u.x.Depth() + 1
}

func (b binary) Depth() uint {
   var xDepth = b.x.Depth()
   var yDepth = b.y.Depth()
   if xDepth > yDepth {
        return xDepth + 1
    } else {
        return yDepth + 1
    }
}

func (m measure) Depth() uint {
    return m.x.Depth() + 1
}

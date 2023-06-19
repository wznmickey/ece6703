import schemdraw
import schemdraw.elements as elm

if __name__ == '__main__':
    d = schemdraw.Drawing()
    d += (a:=elm.Resistor().down().label('500'))
    d += elm.Capacitor().right().label('6e-7')
    d += elm.GroundSignal()
    d += (a:=elm.Resistor().down().label("600").at(a.end))
    d += elm.Capacitor().right().label('1.6e-6')
    d += elm.GroundSignal()
    d += (l:=elm.Resistor().theta(180+45).label("800").at(a.end))
    d += (r:=elm.Resistor().theta(-45).label("800").at(a.end))
    d += elm.Capacitor().left().at(l.end).label("3e-7")
    d += elm.GroundSignal()
    d += elm.Capacitor().right().at(r.end).label('9.5e-7')
    d += elm.GroundSignal()
    d += (r:=elm.Resistor().down().at(r.end).label("600"))
    d += elm.Capacitor().right().label('5.5e-7')
    d += elm.GroundSignal()
    d += (l:=elm.Resistor().theta(180+45).at(r.end).label("600"))
    d += (r:=elm.Resistor().theta(-45).at(r.end).label("1000"))
    d += elm.Capacitor().left().at(l.end).label("2.5e-7")
    d += elm.GroundSignal()
    d += elm.Capacitor().right().at(r.end).label("3.5e-7")
    d += elm.GroundSignal()
    # d += elm.
    # d += elm.Resistor().label('100KΩ')
    # d += elm.Capacitor().down().label('0.1μF', loc='bottom')
    # d += elm.Line().left()
    # d += elm.Ground()
    # d += elm.SourceV().up().label('10V')
    d.draw()
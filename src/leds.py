import math
class DiscoLeds:
    # 6 x 4 tiles
    PANEL_WIDTH = 6
    PANEL_HEIGHT = 4

    # size of each tile
    TILE_WIDTH = TILE_HEIGHT = 4

    # spacing between the tiles.
    Hspace = 3
    Vspace = 3

    def xy(self, x, y):
        """ Convert x,y led coordinate to the lednr on the led-string """

        x = round(x)
        y = round(y)
        if x<0 or x>=self.PANEL_WIDTH*(self.TILE_WIDTH+self.Hspace):
            return -1
        if y<0 or y>=self.PANEL_HEIGHT*(self.TILE_HEIGHT+self.Vspace):
            return -1

        # calculate tile coordinate
        tx = x // (self.TILE_WIDTH+self.Hspace)    # 0..self.PANEL_WIDTH-1
        ty = y // (self.TILE_HEIGHT+self.Vspace)   # 0..self.PANEL_HEIGHT-1
        if ty%2 == 0:
            # flip even rows
            tx = self.PANEL_WIDTH-1-tx

        # calculate subcoordinate of led within the tile.
        _x = x % (self.TILE_WIDTH+self.Hspace)     # 0..self.TILE_WIDTH+self.Hspace-1
        _y = y % (self.TILE_HEIGHT+self.Vspace)    # 0..self.TILE_HEIGHT+self.Vspace-1
        if _y>=self.TILE_HEIGHT: return -1    # spacing
        if _x>=self.TILE_WIDTH: return -1     # spacing
        if _y%2:
            # flip odd rows
            _x = self.TILE_WIDTH-1-_x

        # calculate tile nr
        tilenr = self.PANEL_WIDTH*ty+tx
        # calculate lednr within the tile
        lednr = self.TILE_WIDTH*_y+_x

        # combine
        return tilenr*self.TILE_WIDTH*self.TILE_HEIGHT + lednr

    def printlayout(self):
        # output all lednrs.
        for y in range(self.PANEL_HEIGHT*(self.TILE_HEIGHT+self.Vspace)):
            for x in range(self.PANEL_WIDTH*(self.TILE_WIDTH+self.Hspace)):
                print("%4d" % self.xy(x,y), end='')
            print()


    def fill(self, color=" "):
        self.leds = [color] * (self.PANEL_WIDTH*self.PANEL_HEIGHT * self.TILE_WIDTH*self.TILE_HEIGHT)

    def plot(self, x, y, color):
        i = self.xy(x,y)
        if 0 <= i < len(self.leds):
            self.leds[i] = color

    def printleds(self):
        print("."*(self.PANEL_WIDTH*(self.TILE_WIDTH+self.Hspace)+1))
        for y in range(self.PANEL_HEIGHT*(self.TILE_HEIGHT+self.Vspace)):
            print(".", end="")
            for x in range(self.PANEL_WIDTH*(self.TILE_WIDTH+self.Hspace)):
                i = self.xy(x,y)
                if i==-1:
                    print(".", end="")
                else:
                    print("%s" % self.leds[i], end="")
            print()

    def line(self, x0, y0, x1, y1, color):
        # (x-x0)/(x1-x0) = (y-y0)/(y1-y0)
        if x0==x1:
            for y in range(y0, y1):
                self.plot(x0, y, color)
        elif y0==y1:
            for x in range(x0, x1):
                self.plot(x, y0, color)
        elif abs(x1-x0) > abs(y1-y0):
            # y = (x-x0)*(y1-y0)/(x1-x0) + y0
            for x in range(x0, x1):
                y =  y0 + (x-x0)*(y1-y0)/(x1-x0)
                self.plot(x, y, color)
        else:
            # x = x0 + (y-y0)*(x1-x0)/(y1-y0)
            for y in range(y0, y1):
                x = x0 + (y-y0)*(x1-x0)/(y1-y0)
                self.plot(x, y, color)

    def circle(self, x0, y0, r, color):
        # (x-x0)^2 + (y-y0)^2 = r^2
        for a in range(30):
            self.plot(x0+r*math.cos(a/30*2*math.pi), y0+r*math.sin(a/30*2*math.pi), color)



def main():
    leds = DiscoLeds()
    print("-- layout")
    leds.printlayout()

    print()
    leds.fill(" ")


    print("-- lines")
    leds.line(0,0,30, 15, "*")
    leds.line(10,0,30, 15, "*")
    leds.line(10,0,25, 40, "*")
    leds.printleds()
    print()

    leds.fill(" ")
    leds.circle(10, 10, 5, "+")
    print("-- circle")
    leds.printleds()
    print()

    for x in range(-10, 50):
        leds.fill(" ")
        leds.line(x,-10,x,40, "*")
        print("-- line at x=%d" % x)
        leds.printleds()
        print()

    for y in range(-5, 30):
        leds.fill(" ")
        leds.line(-10,y,40,y, "*")
        print("-- line at y=%d" % y)
        leds.printleds()
        print()

    
if __name__=='__main__':
    main()

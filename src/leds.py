import math
class DiscoLeds:
    # 6 x 4 tiles
    GRIDWIDTH = 6
    GRIDHEIGHT = 4

    # size of each tile
    TILEWIDTH = TILEHEIGHT = 4

    # spacing between the tiles.
    Hspace = 1
    Vspace = 1

    def xy(self, x, y):
        """ Convert x,y led coordinate to the lednr on the led-string """

        x = round(x)
        y = round(y)

        # calculate tile coordinate
        tx = x // (self.TILEWIDTH+self.Hspace)    # 0..self.GRIDWIDTH-1
        ty = y // (self.TILEHEIGHT+self.Vspace)   # 0..self.GRIDHEIGHT-1
        if ty%2 == 0:
            # flip even rows
            tx = self.GRIDWIDTH-1-tx

        # calculate subcoordinate of led within the tile.
        _x = x % (self.TILEWIDTH+self.Hspace)     # 0..self.TILEWIDTH+self.Hspace-1
        _y = y % (self.TILEHEIGHT+self.Vspace)    # 0..self.TILEHEIGHT+self.Vspace-1
        if _y>=self.TILEHEIGHT: return -1    # spacing
        if _x>=self.TILEWIDTH: return -1     # spacing
        if _y%2:
            # flip odd rows
            _x = self.TILEWIDTH-1-_x

        # calculate tile nr
        tilenr = self.GRIDWIDTH*ty+tx
        # calculate lednr within the tile
        lednr = self.TILEWIDTH*_y+_x

        # combine
        return tilenr*self.TILEWIDTH*self.TILEHEIGHT + lednr

    def printlayout(self):
        # output all lednrs.
        for y in range(self.GRIDHEIGHT*(self.TILEHEIGHT+self.Vspace)):
            for x in range(self.GRIDWIDTH*(self.TILEWIDTH+self.Hspace)):
                print("%4d" % self.xy(x,y), end='')
            print()


    def fill(self, color=" "):
        self.leds = [color] * (self.GRIDWIDTH*self.GRIDHEIGHT * self.TILEWIDTH*self.TILEHEIGHT)

    def plot(self, x, y, color):
        i = self.xy(x,y)
        if 0 <= i < len(self.leds):
            self.leds[i] = color

    def printleds(self):
        print("."*(self.GRIDWIDTH*(self.TILEWIDTH+self.Hspace)+1))
        for y in range(self.GRIDHEIGHT*(self.TILEHEIGHT+self.Vspace)):
            print(".", end="")
            for x in range(self.GRIDWIDTH*(self.TILEWIDTH+self.Hspace)):
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
    #leds.printlayout()

    print()
    leds.fill(" ")


    leds.line(0,0,30, 15, "*")
    leds.line(10,0,30, 15, "*")
    leds.line(10,0,25, 40, "*")

    leds.circle(10, 10, 5, "+")
    leds.printleds()

    
if __name__=='__main__':
    main()

auto PPU::serialize(serializer& s) -> void {
  Thread::serialize(s);

  s.integer(this->s.field);
  s.integer(this->s.vtime);
  s.integer((uint&)this->s.pixel.source);
  s.integer(this->s.pixel.color);

  s.integer(l.backColor);
  s.integer(l.screenOneEnable);
  s.integer(l.screenOneMapBase);
  s.integer(l.scrollOneX);
  s.integer(l.scrollOneY);
  s.integer(l.screenTwoEnable);
  s.integer(l.screenTwoMapBase);
  s.integer(l.scrollTwoX);
  s.integer(l.scrollTwoY);
  s.integer(l.screenTwoWindowEnable);
  s.integer(l.screenTwoWindowInvert);
  s.integer(l.screenTwoWindowX0);
  s.integer(l.screenTwoWindowY0);
  s.integer(l.screenTwoWindowX1);
  s.integer(l.screenTwoWindowY1);
  s.integer(l.spriteEnable);
  s.integer(l.spriteWindowEnable);
  s.integer(l.spriteWindowX0);
  s.integer(l.spriteWindowY0);
  s.integer(l.spriteWindowX1);
  s.integer(l.spriteWindowY1);

  s.array(l.sprite);
  s.integer(l.spriteCount);

  for(uint n : range(2)) s.array(l.oam[n]);
  s.integer(l.oamCount);

  s.integer(l.orientation);

  s.integer(r.screenOneEnable);
  s.integer(r.screenTwoEnable);
  s.integer(r.spriteEnable);
  s.integer(r.spriteWindowEnable);
  s.integer(r.screenTwoWindowInvert);
  s.integer(r.screenTwoWindowEnable);
  s.integer(r.backColor);
  s.integer(r.lineCompare);
  s.integer(r.spriteBase);
  s.integer(r.spriteFirst);
  s.integer(r.spriteCount);
  s.integer(r.screenOneMapBase);
  s.integer(r.screenTwoMapBase);
  s.integer(r.screenTwoWindowX0);
  s.integer(r.screenTwoWindowY0);
  s.integer(r.screenTwoWindowX1);
  s.integer(r.screenTwoWindowY1);
  s.integer(r.spriteWindowX0);
  s.integer(r.spriteWindowY0);
  s.integer(r.spriteWindowX1);
  s.integer(r.spriteWindowY1);
  s.integer(r.scrollOneX);
  s.integer(r.scrollOneY);
  s.integer(r.scrollTwoX);
  s.integer(r.scrollTwoY);
  s.integer(r.lcdEnable);
  s.integer(r.lcdContrast);
  s.integer(r.lcdUnknown);
  s.integer(r.icon.sleeping);
  s.integer(r.icon.orientation1);
  s.integer(r.icon.orientation0);
  s.integer(r.icon.auxiliary0);
  s.integer(r.icon.auxiliary1);
  s.integer(r.icon.auxiliary2);
  s.integer(r.vtotal);
  s.integer(r.vsync);
  s.array(r.pool);
  for(uint n : range(16)) s.array(r.palette[n].color);
  s.integer(r.htimerEnable);
  s.integer(r.htimerRepeat);
  s.integer(r.vtimerEnable);
  s.integer(r.vtimerRepeat);
  s.integer(r.htimerFrequency);
  s.integer(r.vtimerFrequency);
  s.integer(r.htimerCounter);
  s.integer(r.vtimerCounter);
}

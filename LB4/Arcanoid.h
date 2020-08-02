#pragma once

using namespace System::Drawing;
using namespace System::Windows::Forms;

ref class ShapeFeature abstract {
  public:
    PointF lcorner;
    SizeF charact_size;
    Brush^ brush;
    virtual void Draw(Graphics^ gr) {}
  protected:
    ShapeFeature() :
    lcorner(PointF::Empty), charact_size(SizeF::Empty), brush(Brushes::White) {}
    ShapeFeature(PointF p, SizeF s, Brush^ br) :
    lcorner(p), charact_size(s), brush(br) {}
    ShapeFeature(ShapeFeature^ obj) {
      operator=(obj);
    }
    virtual void operator= (ShapeFeature^ obj) {
      lcorner = obj->lcorner;
      charact_size = obj->charact_size;
      brush = obj->brush;
    }
};

ref class Character abstract : public ShapeFeature {
  public:
    virtual void Tick() {}
    virtual void Collision(Character^ item) {}
    virtual void Reactions(RectangleF object) {}
    virtual bool isBall() {
      return false;
    }
  protected:
    Character() : ShapeFeature() {}
    Character(PointF p, SizeF s, Brush^ br) : ShapeFeature(p, s, br) {}
    Character(Character^ obj) : ShapeFeature(obj) {
      ShapeFeature::operator=(obj);
    }
    virtual void operator=(Character^ obj) {
      ShapeFeature::operator=(obj); 
    }
};

ref class Landscape abstract : public ShapeFeature {
  public:
    virtual void Collision(Character^ item) {}
    virtual void Reactions(RectangleF object) {}
    virtual int get_number() {
      return 0;
    }
  protected:
    Landscape() : ShapeFeature() {}
    Landscape(PointF p, SizeF s, Brush^ br) : ShapeFeature(p, s, br) {}
    Landscape(Landscape^ obj) : ShapeFeature(obj) {
      ShapeFeature::operator=(obj);
    }
    virtual void operator=(Landscape^ obj) {
      ShapeFeature::operator=(obj);
    }
};

ref class Ball : public Character {
  private:
    double alpha;
    int life;
  public:
    Ball() : Character() {
      alpha = System::Math::PI / 2;
      life = 1;
    }
    Ball(PointF p, SizeF s, Brush^ br, double angle, int _life) : Character(p, s, br) {
      if (angle < 0 || angle > 2 * System::Math::PI) alpha = 7 * System::Math::PI / 4;
      else alpha = angle;
      if (_life > 0) life = _life;
      else life = 1;
    }
    Ball(Ball^ obj) : Character(obj) {
      alpha = obj->alpha;
      life = obj->life;
    }
    virtual void operator=(Ball^ obj) {
      Character::operator=(obj);
      alpha = obj->alpha;
    }
    void operator--(int);
    virtual void Draw(Graphics^ gr) override;
    virtual void Tick() override;
    virtual void Reactions(RectangleF object) override;
    virtual bool isBall() override;
    int get_life();
    void set_alpha(double corner);
};

ref class Platform : public Character {
  private: float vect;
  public:
    Platform() : Character(), vect(0) {}
    Platform(PointF p, SizeF s, Brush^ br) : Character(p, s, br), vect(0) {}
    Platform(Platform^ obj) : Character(obj), vect(obj->vect) {}
    virtual void Draw(Graphics^ gr) override;
    virtual void Tick() override;
    virtual void Collision(Character^ item) override;
    virtual void Reactions(RectangleF object) override;
    void set_vect(float n);
};

ref class Border : public Landscape {
  private:
    bool solid;
  public:
    Border() : Landscape(), solid(true) {}
    Border(PointF p, SizeF s, Brush^ br, bool _solid)
     : Landscape(p, s, br), solid(_solid) {}
    Border(Border^ obj) : Landscape(obj), solid(obj->solid) {}
    virtual void Draw(Graphics^ gr) override;
    virtual void Collision(Character^ item) override;
};

ref class BlocksLine : public Landscape {
private:
  int dist;
  int number;
  Image^ img;
  array<RectangleF>^ blocks;
public:
  BlocksLine(PointF p, SizeF s, Brush^ br, int distance, PictureBox^ mPB) : Landscape(p, s, br) {
    img = nullptr;
    dist = distance;
    number = (int)(mPB->Size.Width / (s.Width + dist));
    blocks = gcnew array<RectangleF>(number);
    for (int i = 0; i < number; ++i) {
      blocks[i] = RectangleF(p, s);
      p.X += s.Width + dist;
    }
  }
  BlocksLine(PointF p, SizeF s, Image^ image, int distance, PictureBox^ mPB) : Landscape(p, s, Brushes::Transparent) {
    img = image;
    dist = distance;
    number = (int)(mPB->Size.Width / (s.Width + dist));
    blocks = gcnew array<RectangleF>(number);
    for (int i = 0; i < number; ++i) {
      blocks[i] = RectangleF(p, s);
      p.X += s.Width + dist;
    }
  }
  BlocksLine(PointF p, SizeF s, Brush^ br, int distance, int numb) : Landscape(p, s, br) {
    img = nullptr;
    dist = distance;
    number = numb;
    blocks = gcnew array<RectangleF>(number);
    for (int i = 0; i < number; ++i) {
      blocks[i] = RectangleF(p, s);
      p.X += s.Width + dist;
    }
  }
  virtual void Draw(Graphics^ gr) override;
  virtual void Collision(Character^ item) override;
  virtual void Reactions(RectangleF object) override;
  virtual int get_number() override;
};

//class conteiner
ref class Field { 
private:
  ref struct move {
    Character^ hero;
    move^ prev;
  };
  ref struct stat {
    Landscape^ type;
    stat^ prev;
  };
  PictureBox^ PB;
  move^ mhead;
  stat^ shead;
public:
  Field(PictureBox^ mPB) {
    PB = mPB;
    mhead = nullptr;
    shead = nullptr;
  }
  void push_back(Character^ item);
  void push_back(Landscape^ item);
  void Draw();
  void Collision();
  void Tick();
  bool isDeath();
  bool isWin();
  void MovePlatform(KeyEventArgs ^e);
  void MovePlatform(MouseEventArgs ^e);
};
#pragma once
#include "Arcanoid.h"

using namespace System::Drawing;
using namespace System::Windows::Forms;

void Ball::operator--(int) {
  life--;
}
void Ball::Draw(Graphics^ gr) {
  gr->FillEllipse(brush, RectangleF(lcorner, charact_size));
}
void Ball::Tick() {
  float speed_up = charact_size.Width / 4;
  lcorner.X += (float)System::Math::Cos(alpha)*speed_up;
  lcorner.Y += (float)System::Math::Sin(alpha)*speed_up;
}
void Ball::Reactions(RectangleF object) {
  RectangleF space(lcorner, charact_size);
  //уменьшение больших углов
  if (alpha >= 2 * System::Math::PI) {
    int a = (int)(alpha / 2 * System::Math::PI);
    alpha -= (a * 2 * System::Math::PI);
  }
  //проверка оси
  RectangleF f360(object.Location, SizeF(object.Width, 3));
  RectangleF s360(PointF(object.X, object.Bottom), SizeF(object.Width, 3));
  if (space.IntersectsWith(f360) || space.IntersectsWith(s360)) {
    alpha = 2 * System::Math::PI - alpha;
  }
  else {
    if (alpha > System::Math::PI) {
      alpha -= System::Math::PI;
      alpha = System::Math::PI - alpha;
      alpha += System::Math::PI;
    }
    else {
      alpha = System::Math::PI - alpha;
    }
  }
  Tick();
}
bool Ball::isBall() {
  return true;
}
int Ball::get_life() {
  return life;
}
void Ball::set_alpha(double corner) {
  alpha = corner;
}

void Platform::Draw(Graphics^ gr) {
  gr->FillRectangle(brush, RectangleF(lcorner, charact_size));
}
void Platform::Tick() {
  int speed = 12;
  lcorner.X += vect*speed;
  vect /= 1.5;
}
void Platform::Collision(Character^ item) {
  RectangleF object(item->lcorner, item->charact_size);
  float pie = charact_size.Width / 5;
  array<RectangleF>^ pies = gcnew array<RectangleF>(5);
  PointF p = lcorner;
  for (int i = 0; i < 5; ++i) {
    pies[i] = RectangleF(p, SizeF(pie, charact_size.Height));
    p.X += pie;
  }
  double alpha = 210 * (System::Math::PI / 180);
  for (int i = 0; i < 5; ++i) {
    if (pies[i].IntersectsWith(object)) {
      if (item->isBall()) {
        Ball^ ball = (Ball^)item;
        ball->set_alpha(alpha);
      }
      break;
    }
    alpha += 30 * (System::Math::PI / 180);
  }
}
void Platform::Reactions(RectangleF object) {
  if (object.X > -5 && object.X < 5) {
    lcorner.X = 0;
  }
  else {
    float right_point = lcorner.X + charact_size.Width;
    if (right_point >= object.X) {
      lcorner.X = object.X - charact_size.Width;
    }
  }
}
void Platform::set_vect(float n) {
  vect = n;
}

void Border::Draw(Graphics^ gr) {
  gr->FillRectangle(brush, RectangleF(lcorner, charact_size));
}
void Border::Collision(Character^ item) {
  RectangleF space(lcorner, charact_size);
  RectangleF object(item->lcorner, item->charact_size);
  if (space.IntersectsWith(object)) {
    if (solid) {
      item->Reactions(space);
      Reactions(object);
    }
    else {
      Ball^ ball = (Ball^)item;
      ball--;
    }
  }
}

void BlocksLine::Draw(Graphics^ gr) {
  if (img) {
    for (int i = 0; i < number; ++i)
      gr->DrawImage(img, blocks[i].Location);
  }
  else gr->FillRectangles(brush, blocks);
}
void BlocksLine::Collision(Character^ item) {
  RectangleF object(item->lcorner, item->charact_size);
  for (int i = 0; i < number; ++i) {
    if (blocks[i].IntersectsWith(object)) {
      if (number > 1) {
        RectangleF tmp = blocks[i];
        blocks[i] = blocks[number - 1];
        blocks[number - 1] = tmp;
      }
      item->Reactions(blocks[number - 1]);
      Reactions(object);
      break;
    }
  }
}
void BlocksLine::Reactions(RectangleF object) {
  blocks->Clear(blocks, number - 1, 1);
  number--;
}
int BlocksLine::get_number() {
  return number;
}

void Field::push_back(Character^ item) {
  move^ tmp = gcnew move;
  tmp->hero = item;
  tmp->prev = mhead;
  mhead = tmp;
}
void Field::push_back(Landscape^ item) {
  stat^ tmp = gcnew stat;
  tmp->type = item;
  tmp->prev = shead;
  shead = tmp;
}
void Field::Draw() {
  Bitmap^ bmp = gcnew Bitmap(PB->Size.Width, PB->Size.Height);
  Graphics^ gr = Graphics::FromImage(bmp);
  move^ mv = mhead;
  while (mv) {
    mv->hero->Draw(gr);
    mv = mv->prev;
  }
  stat^ sv = shead;
  while (sv) {
    sv->type->Draw(gr);
    sv = sv->prev;
  }
  PB->Image = bmp;
}
void Field::Collision() {
  move^ mv = mhead;
  stat^ st = shead;
  while (st) {
    while (mv) {
      st->type->Collision(mv->hero);
      move^ tmp = mv->prev;
      while (tmp) {
        mv->hero->Collision(tmp->hero);
        tmp = tmp->prev;
      }
      mv = mv->prev;
    }
    mv = mhead;
    st = st->prev;
  }
}
void Field::Tick() {
  move^ start = mhead;
  while (mhead) {
    mhead->hero->Tick();
    mhead = mhead->prev;
  }
  mhead = start;
  Collision();
}
bool Field::isDeath() {
  move^ mv = mhead;
  while (mv) {
    if (mv->hero->isBall()) {
      Ball^ tmp = (Ball^)mv->hero;
      if (!tmp->get_life()) return true;
    }
    mv = mv->prev;
  }
  return false;
}
bool Field::isWin() {
  stat^ st = shead;
  while (st) {
    if (st->type->get_number()) return false;
    st = st->prev;
  }
  return true;
}
void Field::MovePlatform(KeyEventArgs ^e) {
  move^ tmp = mhead;
  while (tmp) {
    if (!tmp->hero->isBall()) {
      Platform^ pl = (Platform^)tmp->hero;
      if (e->KeyCode == Keys::Right) {
        pl->set_vect(1);
      }
      if (e->KeyCode == Keys::Left) {
        pl->set_vect(-1);
      }
    }
    tmp = tmp->prev;
  }
}
void Field::MovePlatform(MouseEventArgs ^e) {
  move^ tmp = mhead;
  while (tmp) {
    if (!tmp->hero->isBall()) {
      Platform^ pl = (Platform^)tmp->hero;
      pl->lcorner.X = e->Location.X;
    }
    tmp = tmp->prev;
  }
}
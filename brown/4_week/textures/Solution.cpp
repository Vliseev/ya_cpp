// This is an open source non-commercial project. Dear PVS-Studio, please check
// it. PVS-studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "Common.h"

using namespace std;
static const char default_sym = '.';

inline bool IsPointInRec(Point p, Size size) {
  return p.x <= size.width && p.y <= size.height;
}

// Этот файл сдаётся на проверку
// Здесь напишите реализацию необходимых классов-потомков `IShape`

class Shape : public IShape {
 protected:
  std::shared_ptr<ITexture> texture__;
  Point point__;
  Size size__;

 public:
  void SetTexture(std::shared_ptr<ITexture> ptr) override {
    texture__ = move(ptr);
  }

  ITexture *GetTexture() const override {
    return texture__.get();
  }

  void SetPosition(Point point) override {
    point__ = point;
  }

  Point GetPosition() const override {
    return point__;
  }

  void SetSize(Size size) override {
    size__ = size;
  }

  Size GetSize() const override {
    return size__;
  }

  void PaintPixel(Image &image, const Size &t_size, const Image *&t_ptr,
                  int i_x, int i_y) const {
    if (t_ptr && (i_x - point__.x < t_size.width) &&
        (i_y - point__.y < t_size.height)) {
      image[i_y][i_x] = (*t_ptr)[i_y - point__.y][i_x - point__.x];
    } else {
      image[i_y][i_x] = default_sym;
    }
  }

  virtual bool InShape(Point p, Size size) const = 0;

  void Draw(Image &image) const override {
    auto x_bound = min<int>(image[0].size(), point__.x + size__.width);
    auto y_bound = min<int>(image.size(), point__.y + size__.height);

    Size text_size{};
    const Image *im_ptr = nullptr;

    if (texture__) {
      text_size = texture__->GetSize();
      im_ptr = addressof(texture__->GetImage());
    }

    for (int i_y = point__.y; i_y < y_bound; ++i_y)
      for (int i_x = point__.x; i_x < x_bound; ++i_x) {
        auto p = Point({i_x - point__.x, i_y - point__.y});
        if (InShape(p, size__)) PaintPixel(image, text_size, im_ptr, i_x, i_y);
      }
  }
};

class RecShape : public Shape {
  bool InShape(Point p, Size size) const override {
    return IsPointInRec(p, size__);
  }

  unique_ptr<IShape> Clone() const override {
    auto clone_shape = make_unique<RecShape>(*this);
    return clone_shape;
  }
};

class EllShape : public Shape {
  bool InShape(Point p, Size size) const override {
    return IsPointInEllipse(p, size__);
  }

  unique_ptr<IShape> Clone() const override {
    auto clone_shape = make_unique<EllShape>(*this);
    return clone_shape;
  }
};

// Напишите реализацию функции
unique_ptr<IShape> MakeShape(ShapeType shape_type) {
  if (shape_type == ShapeType::Ellipse) {
    return make_unique<EllShape>();
  } else if (shape_type == ShapeType::Rectangle) {
    return make_unique<RecShape>();
  } else {
    throw logic_error("Invalid shape_type");
  }
}
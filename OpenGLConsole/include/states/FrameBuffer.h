
namespace coreone::graphics
{
  class Texture;
}

using namespace coreone::graphics;

namespace core
{
  class FrameBuffer
  {
  public:
    FrameBuffer();
    virtual ~FrameBuffer();

    void AddTextureAttachement(const Texture& texture);

    virtual void SetUpAttachements();

    void Use();
    void Unuse();

  private:
    unsigned int _framebuffer;
    unsigned int _colorbuffer;
    unsigned int _Rbo;
  };
}
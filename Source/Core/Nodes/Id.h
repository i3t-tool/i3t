namespace Core
{
typedef unsigned long long ID;

class IdGenerator
{
  static ID m_id;

public:
  static ID next();
};
}

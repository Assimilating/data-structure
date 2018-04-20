class Int {
private:
  int val;
public:
  Int(int input=0) { val = input; }

  int key() const { return val; }

  bool   operator = (int input) { val = input; }
};


int Int:: ostream& operator<<(ostream& s, const Int& i)
  { return s << i.key(); }
int Int:: ostream& operator<<(ostream& s, const Int* i)
  { return s << i->key(); }

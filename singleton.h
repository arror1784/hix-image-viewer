template <typename T>
class Singleton
{
public:
	static T& getInstance()
	{
		static T    instance; // Guaranteed to be destroyed.
								// Instantiated on first use.
		return instance;
	}
	Singleton(Singleton const&) = delete;
	Singleton& operator=(Singleton const&) = delete;
	Singleton() {}
	~Singleton() {}

};
#ifndef _CXX_CONFIG_H_
#define _CXX_CONFIG_H_ 1
#include "ITree.h"
#include <cctype>
#include <cstring>
#include <fmt/format.h>
#include <limits>
#include <map>
#include <string>
#include <typeinfo>

#ifdef __cpp_lib_optional
#include <optional>
#endif

#if __cplusplus >= 201603L
#define CXXCONF_NODISCARD [[nodiscard]]
#else
#define CXXCONF_NODISCARD
#endif

struct json_object;
namespace cxxconfig {

	class AbstractValue {};

	template <typename T> class ValueType : public AbstractValue {
	  public:
		ValueType() = delete;
		ValueType(const T &t) : value(t) {}

		ValueType &operator=(const ValueType &other) = default;

		const T &getValue() const noexcept {
#ifdef CXXCONF_NO_RTTI
			return static_cast<const T &>(this->value);
#else
			return reinterpret_cast<const T &>(this->value);
#endif
		}

		// T getValue() noexcept { return this->value; }

		// 		template <typename U> const U &as() const {
		// 			// if (m_value == nullptr) {
		// 			// 	throw std::domain_error("No value");
		// 			// }
		virtual void encode(std::string &code) {}
		void decode(const std::string &code) {}

		virtual unsigned int getHashCode() { return typeid(T).hash_code(); }

	  private:
		T value;
	};
	//	template void ValueType::decode<int>(const std::string &code);
	using float_t = float;
	using int_t = int;
	using string_t = std::string;
	using bool_t = bool;

	template class ValueType<float_t>;
	template class ValueType<int_t>;
	template class ValueType<string_t>;
	template class ValueType<bool_t>;

	// class Config;
	// template class ValueType<Config>;

	class Random {};

	/**
	 *
	 */
	class Config : public ITree<Config> {
	  public:
		Config() {}
		Config(const Config &other) {}
		Config(Config &&other) {}
		virtual ~Config() {}

		Config &operator=(const Config &other) { return *this; }
		Config &operator=(Config &&other) { return *this; }

	  public: /*	Get and set methods.	*/
		/**
		 * @brief
		 *
		 */
		enum ConfigFormat {
			Unknown = 0,
			XML = 1,  /*  */
			YAML = 2, /*  */
			JSON = 3, /*  */
			INI = 4,
			Custom = 1000
		};

		template <class T> const T &get(const std::string &key) const {
			/*	Check if valid type.	*/
			// ValueType<T> v;

			static_assert(std::is_class<ValueType<T>>::value || std::is_floating_point<T>::value ||
							  std::is_integral<T>::value,
						  "");

			const std::type_info &i = typeid(T);
			const auto typehash = i.hash_code();

			return this->getInternal<T>(key);
		}

		template <class T> void set(const std::string &key, const T &value) /*noexcept(noexcept(isSet(key)))*/ {
			static_assert(std::is_class<ValueType<T>>::value || std::is_floating_point<T>::value ||
							  std::is_integral<T>::value,
						  "Must be a supported type");
			auto v = new ValueType<T>(value);

			va_va[key] = static_cast<AbstractValue *>(v);
			// this->setInternal(key, (const void *)&value, type);
		}

		// template <class T> T *getBlob(const std::string &key, size_t *size) const {
		// 	static_assert(std::is_floating_point<T>::value || std::is_integral<T>::value, "");
		// 	T tmp;
		// 	// this->getInternal(key, (void *)&tmp, type);
		// 	return tmp;
		// }

		// template <class T> void setBlob(const std::string &key, const T &value, size_t t) {
		// 	const std::type_info &type = typeid(T);
		// 	// this->setInternal(key, (const void *)&value, type);
		// }

		/**
		 * Get sub configuration node space.
		 * @param key
		 * @return
		 */
		virtual Config &getSubConfig(const std::string &key) {}
		virtual bool tryGetSubConfig(const std::string &key, Config &config) {
			if (this->getNumChildren() <= 0)
				return false;

			// = get<Config>(key);
			get<int>("a");
			set<Random>("B", Random());

			return true;
		}

		/**
		 * Check if there exist a element with specific
		 * key.
		 * @param key string key.
		 * @return true if exist, false otherwise.
		 */
		virtual bool isSet(const std::string &key) { return this->iconfig.find(key) != this->iconfig.end(); }

		/**
		 * Print all elements in the configuration
		 * object.
		 */
		virtual void printTable() const {}

		/**
		 * Print all elements in the configuration
		 * object.
		 * @param file
		 */
		// virtual void printTable(FILE *IO) const;	//TODO change to the reference to make it thread safe.
		// TODO add support for the IO object.
		// virtual void printTable(Ref<IO> &io) const;

		// // TODO deal with the name.
		// virtual void save(Ref<IO> &io, ConfigFormat format);

		// virtual void parseConfigFile(Ref<IO> &io, ConfigFormat format = ConfigFormat::Unknown);

		// virtual size_t getNrElements() const noexcept;
		// virtual const std::vector<std::string&>& getKeys() const;

	  private: /*	Internal methods.	*/
		/**
		 *	Set internal configuration value.
		 *
		 *	\key key associated with the value.
		 *
		 *	\value pointer to the value.
		 *
		 *	\type object describing the value data type.
		 */

		template <typename T> void setInternal(const std::string &key) {}

		template <typename T> T &getInternal(const std::string &key) const {

			// /*  Supported data types.   */
			const std::type_info &i = typeid(int);
			// const std::type_info &f = typeid(float);
			// const std::type_info &s0 = typeid(std::string);
			// const std::type_info &s1 = typeid(char);
			// const std::type_info &b = typeid(bool);
			// const std::type_info &s2 = typeid(const char *);

			// /*	*/
			// std::map<std::string, int>::const_iterator iti;
			// std::map<std::string, float>::const_iterator fti;
			// std::map<std::string, std::string>::const_iterator sti;

			// /*	*/
			// if (i == type) {
			// 	iti = iconfig.find(key);
			// 	if (iti != iconfig.end())
			// 		*((int *)value) = (*iti).second;
			// 	else
			// 		throw std::invalid_argument(fmt::format("Key '{}' don't exists", key.c_str()));
			// } else if (type == b) {
			// 	iti = iconfig.find(key);
			// 	if (iti != iconfig.end())
			// 		*((bool *)value) = (bool)(*iti).second;
			// 	else
			// 		throw std::invalid_argument(fmt::format("Key '{}' don't exists", key.c_str()));
			// } else if (f == type) {
			// 	fti = fconfig.find(key);
			// 	if (fti != fconfig.end())
			// 		*((float *)value) = (*fti).second;
			// 	else
			// 		throw std::invalid_argument(fmt::format("Key '{}' don't exists", key.c_str()));
			// } else if (s0 == type) {
			// 	sti = sconfig.find(key);
			// 	if (sti != sconfig.end())
			// 		*((std::string *)value) = (*sti).second;
			// 	else
			// 		throw std::invalid_argument(fmt::format("Key '{}' don't exists", key.c_str()));
			// } else if (s2 == type) {
			// 	sti = sconfig.find(key);
			// 	if (sti != sconfig.end())
			// 		*((const char **)value) = (*sti).second.c_str();
			// 	else
			// 		throw std::invalid_argument(fmt::format("Key '{}' don't exists", key.c_str()));
			// } else if (type.before(s1)) {

			// } else
			// 	throw std::invalid_argument(fmt::format("Invalid argument type '{}'.", type.name()));
		}

		/**
		 *	Get configuration value.
		 *
		 *	\key key associated with the value.
		 *
		 *	\value pointer to the value.
		 *
		 *	\type object describing the value data type.
		 */
		// template <typename T> void getInternal(const std::string &key, void *value, const std::type_info &type)
		// const;

		// template <class T> class IConfigIterator : public Iterator<T> { /*	*/
		// };
		// TODO add iterator for config arguments.
		// virtual Iterator getIterator();

		// virtual void setName(const std::string &name);
		// virtual const std::string &getName() const noexcept;
		// virtual std::string getName() noexcept;

	  protected:
		// virtual void parse_xml(Ref<IO> &io);
		// virtual void parse_yaml(Ref<IO> &io);
		// virtual void parse_json(Ref<IO> &io);
		// virtual void save_xml(Ref<IO> &io);
		// virtual void save_yaml(Ref<IO> &io);
		// virtual void save_json(Ref<IO> &io);

	  private:
		// TODO deal with the type.
		// static void save_json_recursive_config(const Config *config, struct json_object *root);
		// static void parse_json_recursive_config(Config *config, struct json_object *root);

	  public:
		// virtual Config *getSuperInstance();

	  protected: /*	*/
		/*  Config tree hierarchy.  */
		std::map<std::string, ITree<Config> *> mapSubConfig; /*  */
		std::map<std::string, AbstractValue *> va_va;
		// std::map<std::string, ValueContainer &> lookup;

		/*  List of all attributes. */
		std::map<std::string, int> iconfig;			/*	Integer.	*/
		std::map<std::string, float> fconfig;		/*	Float.	*/
		std::map<std::string, std::string> sconfig; /*	String.	*/
		// std::map<unsigned int, ValueContainer> values;
		typedef struct blob_t {
			size_t nbytes;
			void *blob;
		} Blob;
		std::map<std::string, Blob> bconfig;
	};

} // namespace cxxconfig

#endif

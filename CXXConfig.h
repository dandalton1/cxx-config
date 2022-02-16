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

	template <typename T> class ValueType;

	class AbstractValue {
	  public:
		template <class T> const ValueType<T> &as() const { return reinterpret_cast<const ValueType<T> &>(*this); }
		template <class T> ValueType<T> &as() { return reinterpret_cast<ValueType<T> &>(*this); }
		// template <class T> const ValueType<T> &as_ref() const { return reinterpret_cast<const ValueType<T>>(*this); }
		// template <class T> const ValueType<T> &as_ptr() const { return reinterpret_cast<const ValueType<T>>(this); }
	};

	template <typename T> class ValueType : public AbstractValue {
	  public:
		using ObjecType = T;
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
		// virtual void encode(std::string &code) {}
		// void decode(const std::string &code) {}

		virtual unsigned int getHashCode() { return typeid(T).hash_code(); }

	  private:
		T value;
	};
	//	template void ValueType::decode<int>(const std::string &code);
	using float_t = float;
	using int_t = int;
	using string_t = std::string;
	using bool_t = bool;
	typedef struct blob_t {
		size_t nbytes;
		void *blob;
	} Blob;

	template class ValueType<float_t>;
	template class ValueType<int_t>;
	template class ValueType<string_t>;
	template class ValueType<bool_t>;
	template class ValueType<Blob>;

	class IConfigBase {
	  public:
	};

	/**
	 * @brief
	 *
	 */
	class IConfig : public ITree<ValueType<IConfigBase>>, public ValueType<IConfigBase> {
	  public:
		IConfig(IConfig *parent = nullptr) : ValueType(*this) {}
		//IConfig(const IConfig &other) {}
		//IConfig(IConfig &&other) {}
		virtual ~IConfig() {}

		IConfig &operator=(const IConfig &other) { return *this; }
		IConfig &operator=(IConfig &&other) { return *this; }

	  public: /*	Get and set methods.	*/
		const AbstractValue &operator[](const std::string &key) { return *this->va_va[key]; }

		template <class T> T get(const std::string &key) const {
			/*	Check if valid type.	*/
			// ValueType<T> v;
			// TODO add check if of inherit type of...
			static_assert(std::is_class<ValueType<T>>::value || std::is_floating_point<T>::value ||
							  std::is_integral<T>::value,
						  "");
			return this->va_va.at(key)->as<T>().getValue();
		}
		template <class T> T &get_ref(const std::string &key) const {}

		template <class T> void set(const std::string &key, const T &value) /*noexcept(noexcept(isSet(key)))*/ {
			static_assert(std::is_class<ValueType<T>>::value || std::is_floating_point<T>::value ||
							  std::is_integral<T>::value,
						  "Must be a supported type");
			auto v = new ValueType<T>(value);

			va_va[key] = static_cast<AbstractValue *>(v);
		}

		/**
		 * Get sub configuration node space.
		 * @param key
		 * @return
		 */
		virtual IConfig &getSubConfig(const std::string &key) {}
		virtual bool tryGetSubConfig(const std::string &key, IConfig &config) {
			if (this->getNumChildren() <= 0)
				return false;
			return true;
		}

		/**
		 * Check if there exist a element with specific
		 * key.
		 * @param key string key.
		 * @return true if exist, false otherwise.
		 */
		virtual bool isSet(const std::string &key) { return this->va_va.find(key) != this->va_va.end(); }

		// virtual std::map<std::string, AbstractValue *>::const_pointer begin() const { return
		// this->va_va.cbegin(); }

		virtual void setName(const std::string &name) {}
		virtual const std::string &getName() const noexcept {}
		virtual std::string getName() noexcept {}

	  protected: /*	*/
		/*  Config tree hierarchy.  */
		std::map<std::string, ITree<IConfig> *> _mapSubConfig; /*  */
		std::map<std::string, AbstractValue *> va_va;

		std::map<std::string, Blob> bconfig;
	};

	class CXXConfig : public IConfig {
	  public:
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

	  protected:
		// virtual void parse_xml(Ref<IO> &io);
		// virtual void parse_yaml(Ref<IO> &io);
		// virtual void parse_json(Ref<IO> &io);
		// virtual void save_xml(Ref<IO> &io);
		// virtual void save_yaml(Ref<IO> &io);
		// virtual void save_json(Ref<IO> &io);

	  public:
		// virtual Config *getSuperInstance();

	  private:
		// TODO deal with the type.
		// static void save_json_recursive_config(const Config *config, struct json_object *root);
		// static void parse_json_recursive_config(Config *config, struct json_object *root);
	};

} // namespace cxxconfig

#endif

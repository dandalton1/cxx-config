#ifndef _CXX_CONFIG_EXPORT_IMPORT_H_
#define _CXX_CONFIG_EXPORT_IMPORT_H_ 1
#include "CXXConfig.h"
//#include"Exce

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

	class CXXConfigExportImport {
	  public:
		/**
		 * @brief
		 *
		 */
		enum ConfigFormat : size_t {
			Unknown = 0,
			XML = 1,  /*  */
			YAML = 2, /*  */
			JSON = 3, /*  */
			INI = 4,
			Custom = 1000
		};

		CXXConfigExportImport() = default;

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
		virtual void printTable(FILE *IO) const; // TODO change to the reference to make it thread safe.
		// TODO add support for the IO object.
		// virtual void printTable(Ref<IO> &io) const;

		// // TODO deal with the name.
		virtual void save(IConfig &config, const char *path, ConfigFormat format);
		virtual IConfig parse(const char *path);

		// virtual void parseConfigFile(Ref<IO> &io, ConfigFormat format = ConfigFormat::Unknown);

	  protected:
		virtual void parse_xml(IConfig &config, Ref<IO> &io);
		virtual void parse_yaml(IConfig &config, Ref<IO> &io);
		virtual void parse_json(IConfig &config, Ref<IO> &io);
		virtual void save_xml(IConfig &config, Ref<IO> &io);
		virtual void save_yaml(IConfig &config, Ref<IO> &io);
		virtual void save_json(IConfig &config, Ref<IO> &io);

	  private:
		// TODO deal with the type.
		static void save_json_recursive_config(const IConfig *config, struct json_object *root);
		static void parse_json_recursive_config(IConfig *config, struct json_object *root);
	};

} // namespace cxxconfig

#endif
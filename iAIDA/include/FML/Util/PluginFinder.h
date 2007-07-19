#ifndef PLUGINFINDER_H
#define PLUGINFINDER_H

/**
 * Plugin Mechanism,
 *
 * \author Jakub T. Moscicki, CERN 2002
 */

# include "FML/Util/Plugin.h"

# include <iostream>
# include <string>
# include <vector>

//# include "boost/utility.hpp"

namespace Util
{

  typedef std::string uri_string_t;

  /**
   * PluginLocator  maps plugin  resource  identifier (IRes+ID) 
   * into  URI of the component in which to find the plugin.
   *
   * PluginLocator maintains a small  database which is typically saved
   * in  the  file on  disk,  in a well  known  place  (e.g. users  home
   * directory).   However  user  may  serialize and  deserialize  the
   * database to/from any iostream.
   *
   * Optional information is stored together with uri. The type of optional
   * info is by default string but may be changed to any type which may be
   * stringified using Util::to_string() function.
   *
   */
  template<class IRes, class ID = std::string, class OPT = std::string>
  class PluginLocator
  {
  public:
    PluginLocator();
    /// Read the XML contents. If stream is empty or constisting only from 
    /// white space then it is OK.
    PluginLocator(std::istream& streamIn);
    ~PluginLocator();

    //    bool streamIn(std::istream&);
    bool streamOut(std::ostream&);
    
    /*may be relative path,
      here we break existing interface, attention for OPT == bool*/
    bool         setURI(const ID& id, 
			uri_string_t uri, const OPT& opt = OPT(),
			bool replace = false);

    void         eraseURI(const ID& id);

    /// Return empty string if id not found.
    uri_string_t getURI(const ID& id) const;

    /// Return OPT() if not found.
    OPT          getOPT(const ID& id) const;

    /// Get all identifiers stored in the locator.
    std::vector<ID> getIDVector() const;

  private:
    bool _streamIn(std::istream& is);

    typedef std::map<ID,std::pair<uri_string_t,OPT> > urimap_t;
    // many ID -> one uri
    urimap_t m_urimap;
  };

  /**
   * PluginLoader loads component which contains requested plugin
   * (component is found by PluginLocator).
   *
   */
  template<class IRes, class ID = std::string, class OPT = std::string>
    //  class PluginLoader : boost::noncopyable    
    class PluginLoader 
  {
  public:
    PluginLoader();

    /// Persistency. Load the plugins from the list. Stream-in the locator.
    PluginLoader(std::istream& is);

    // Unload all loaded components.
    ~PluginLoader();

    /** 
     * Persistency (save list of currently loaded plugins).
     * Stream-out the locator too.
     */
    bool streamOut(std::ostream& os);

    /// Locator type (help for generic programming).
    typedef PluginLocator<IRes,ID,OPT> Locator; 

    /// Map plugin ids to component URIs.
    Locator& locator();
 
     /// Map plugin ids to component URIs.
    const Locator& locator() const;
  
    /**
     * Find the component uri for given plugin id and load the component.
     * Make sure that component exists (for example shared library) and
     * that is readable (accessible).
     * Return false if loading component failed. 
     * If plugin already loaded (present in the registry) then the call is
     * no-op and return false.
     *
     * Use system PATH mechanisms (on Linux LD_LIBRARY_PATH).
     */
    bool loadPluginComponent(const ID& id);

    /**
     * Unload the plugin and its component. Every load call must have 
     * corresponding unload call. 
     *
     * Return false if plugin is not registered in the registry 
     * (was not loaded) or there was an error in underlying system call
     * to unload the shared library. On Linux: dlclose on link time shlib
     * does not unlink it.
     *
     * Some plugins may specify their own PointerPolicy to protect unloading
     * the plugin if it is still in use by client code. In such a case
     * unload of the plugin will fail and is treated as no-op.
     * By default no checking is done.
     *
     * Finally there may be several plugins loaded in the same component.
     * The component will not be *physically* unloaded until there are other
     * plugins which use it. In this case return true.
     *
     */
    bool unloadPluginComponent(const ID& id);

    /**
     * Error type.
     */
    enum err_t { no_error = 0, 
		 plugin_already_registered, 
		 plugin_not_registered,
		 plugin_share_violation, //< plugin in use - unable to unload.
		 system_error,           //< system error (ldopen,ldclose).
		 /// locator errors.
		 component_not_found,    //< component uri not found.
		 component_has_no_plugin //< plugin is not defined in the component.
    };

    /**
     * Get last error. 
     */
    std::pair<err_t,std::string> lastError();

  private:

  PluginLoader(const  PluginLoader & ) { }
  void operator=(const  PluginLoader &) {}

    // PENDING: load components multiple times -> exactly the same state?
    // or load once only?
    bool _streamIn(std::istream& is);

    // this implementation takes advantage of Linux ld feature:
    // internal count on ldopen, ldclose.
    // for other OSes some explicit mechanisms may be needed.

    // OS dependent
    typedef void* shlib_handle_t;

# if 0
    // PENDING: why we store uri as keys and not the plugin ids?
    typedef std::map<ID, std::pair<shlib_handle_t, int> > comp_map_t;
# endif

    typedef std::map<ID, shlib_handle_t> comp_map_t;

    // one URI -> many handle values -> one physical component (shlib file)
    comp_map_t m_loaded_components;

    std::pair<err_t,std::string> m_last_error;

    Locator m_locator;
  };
}

# include "PluginFinder.inl"


#endif

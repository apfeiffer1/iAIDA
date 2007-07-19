#ifndef AIDA_PLUGIN_AIDA_PLUGIN_INFO_H
# define AIDA_PLUGIN_AIDA_PLUGIN_INFO_H 1


//# include "AIDA_Plugin/config.h"
# include "PluginManager/PluginInfo.h"


namespace AIDA_Plugin { 



class AIDA_PluginType;



class  AIDA_PluginInfo : public seal::PluginInfo
{
public:
    typedef AIDA_PluginType Object;
    struct  Factory {
	virtual ~Factory (void);
	virtual Object *create ( ) = 0;
    };

    /** Concrete #Factory implementation for a specific object type @c T.  */
    template <class T> struct AutoFactory : public Factory {
	virtual Object *create ( ) { return new T ( ); }
    };

     AIDA_PluginInfo (seal::Module *module, const std::string &type);
     AIDA_PluginInfo (seal::Module *module, seal::ModuleDescriptor *details);
    ~AIDA_PluginInfo (void);

    AIDA_PluginType *	create () const;
    void		attach (Factory *factory);
    virtual void	detach (void);

private:
    Factory *		factory (void) const;

    Factory		*m_factory;
};


} // end namespace AIDA_Plugin 


#endif // PLUGIN_MANAGER_TEST_PLUGIN_INFO_H

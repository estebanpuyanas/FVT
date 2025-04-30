#ifndef VERSION_CONTROL_FEATURE_H
#define VERSION_CONTROL_FEATURE_H

#include <string>

/**
* @brief Base class for all version control entities (Commit, Branch, etc) 
*/
class version_control_feature 
{
protected:
    std::string name;

public: 
    version_control_feature(const std::string& feature_name) : name(feature_name) {}

    /**
     * @brief Gets the name of the version control entity.
     * @return The entity name.
     */
    std::string get_name() const { return name; }

    /**
     * @brief Virtual destructor, ensure proper cleanup of derived classes.
     */
    virtual ~version_control_feature() {}

    /**
     * @brief Implemented in derived classes.
     */
    virtual void display_info() const = 0;
};

#endif
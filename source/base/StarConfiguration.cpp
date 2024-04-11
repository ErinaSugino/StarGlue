#include "StarConfiguration.hpp"
#include "StarFile.hpp"
#include "StarLogging.hpp"

namespace Star {

Configuration::Configuration(Json defaultConfiguration, Json currentConfiguration)
  : m_defaultConfig(defaultConfiguration), m_currentConfig(currentConfiguration) {}

Json Configuration::defaultConfiguration() const {
  return m_defaultConfig;
}

Json Configuration::currentConfiguration() const {
  return m_currentConfig;
}

Json Configuration::get(String const& key) const {
  MutexLocker locker(m_mutex);
  return m_currentConfig.get(key, {});
}

Json Configuration::getPath(String const& path) const {
  MutexLocker locker(m_mutex);
  return m_currentConfig.query(path, {});
}

Json Configuration::getDefault(String const& key) const {
  MutexLocker locker(m_mutex);
  return m_defaultConfig.get(key, {});
}

Json Configuration::getDefaultPath(String const& path) const {
  MutexLocker locker(m_mutex);
  return m_defaultConfig.query(path, {});
}

void Configuration::set(String const& key, Json const& value) {
  MutexLocker locker(m_mutex);
  if (key == "configurationVersion")
    throw ConfigurationException("cannot set configurationVersion");

  m_currentConfig = m_currentConfig.set(key, value);
}

void Configuration::setPath(String const& path, Json const& value) {
  MutexLocker locker(m_mutex);
  if (path.splitAny("[].").get(0) == "configurationVersion")
    throw ConfigurationException("cannot set configurationVersion");

  m_currentConfig = m_currentConfig.setPath(path, value);
}

}

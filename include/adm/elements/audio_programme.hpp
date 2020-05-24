/// @file audio_programme.hpp
#pragma once

#include <boost/optional.hpp>
#include <memory>
#include <vector>
#include "adm/elements/time.hpp"
#include "adm/elements/audio_content.hpp"
#include "adm/elements/audio_programme_id.hpp"
#include "adm/elements/audio_programme_ref_screen.hpp"
#include "adm/elements/loudness_metadata.hpp"
#include "adm/elements_fwd.hpp"
#include "adm/helper/element_range.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/named_type.hpp"
#include "adm/detail/parameter_store.hpp"
#include "adm/export.h"

namespace adm {

  class Document;
  /// \addtogroup AudioProgramme
  ///@{
  /// @brief Tag for NamedType ::AudioProgrammeName
  struct AudioProgrammeNameTag {};
  /// @brief NamedType for the audioProgrammeName attribute
  using AudioProgrammeName =
      detail::NamedType<std::string, AudioProgrammeNameTag>;
  /// @brief Tag for NamedType ::AudioProgrammeLanguage
  struct AudioProgrammeLanguageTag {};
  /// @brief NamedType for the language attribute of the audioProgramme element
  using AudioProgrammeLanguage =
      detail::NamedType<std::string, AudioProgrammeLanguageTag>;
  /// @brief Tag for NamedType ::MaxDuckingDepth
  struct MaxDuckingDepthTag {};
  /**
   * @brief NamedType for the maxDuckingDepth attribute element
   *
   * Valid values are in the range [-62, 0]
   */
  using MaxDuckingDepth = detail::NamedType<double, MaxDuckingDepthTag,
                                            detail::RangeValidator<-62, 0>>;

  struct AudioProgrammeDefaults {
    static Start create(Start::tag) { return Start{std::chrono::seconds(0)}; }
  };

  /// @brief Tag for AudioProgramme
  struct AudioProgrammeTag {};
  /**
   * @brief Class representation of the audioProgramme ADM element
   *
   * @warning This class has unsupported parameters:
   *   - AudioProgrammeReferenceScreen
   *
   * @headerfile audio_programme.hpp <adm/elements/audio_programme.hpp>
   */
  class AudioProgramme : public std::enable_shared_from_this<AudioProgramme> {
    using ManditoryParameters =
        ParameterList<AudioProgrammeId, AudioProgrammeName>;
    using OptionalParameters =
        ParameterList<AudioProgrammeLanguage, End, MaxDuckingDepth,
                      LoudnessMetadata, AudioProgrammeReferenceScreen>;
    using ParametersWithDefaults = ParameterList<Start>;
    using AudioProgrammeParameterStore =
        detail::ParameterStore<ManditoryParameters, OptionalParameters,
                               ParametersWithDefaults, AudioProgrammeDefaults>;

   public:
    typedef AudioProgrammeTag tag;
    /// @brief Type that holds the id for this element;
    typedef AudioProgrammeId id_type;

    /**
     * @brief Static create function template
     *
     * Templated static create function which accepts a variable number of
     * ADM parameters in random order after the mandatory ADM parameters. The
     * actual constructor is private. This way it is ensured, that an
     * AudioProgramme object can only be created as a `std::shared_ptr`.
     */
    template <typename... Parameters>
    static std::shared_ptr<AudioProgramme> create(
        AudioProgrammeName name, Parameters... optionalNamedArgs);

    /**
     * @brief Copy AudioProgramme
     *
     * The actual copy constructor is private to ensure that an AudioProgramme
     * can only be created as a `std::shared_ptr`. This is not a deep copy! All
     * referenced objects will be disconnected.
     */
    ADM_EXPORT std::shared_ptr<AudioProgramme> copy() const;

    /**
     * @brief ADM parameter getter template
     *
     * Templated getter with the wanted ADM parameter type as template
     * argument. If currently no value is available trying to get the adm
     * parameter will result in an exception. Check with the has method
     * before
     */
    template <typename Parameter>
    Parameter get() const {
      static_assert(
          AudioProgrammeParameterStore::isValidParameter<Parameter>::value,
          "Not a valid AudioProgramme parameter");
      return storage_.get<Parameter>();
    }

    /**
     * @brief ADM parameter has template
     *
     * Templated has method with the ADM parameter type as template argument.
     * Returns true if the ADM parameter is set or has a default value.
     */
    template <typename Parameter>
    bool has() const {
      static_assert(
          AudioProgrammeParameterStore::isValidParameter<Parameter>::value,
          "Not a valid AudioProgramme parameter");
      return storage_.has<Parameter>();
    }

    /**
     * @brief ADM parameter isDefault template
     *
     * Templated isDefault method with the ADM parameter type as template
     * argument. Returns true if the ADM parameter is the default value.
     */
    template <typename Parameter>
    bool isDefault() const {
      static_assert(
          AudioProgrammeParameterStore::isValidParameter<Parameter>::value,
          "Not a valid AudioProgramme parameter");
      return storage_.isDefault<Parameter>();
    }

    template <typename Parameter>
    void set(const Parameter &value) {
      static_assert(
          AudioProgrammeParameterStore::isValidParameter<Parameter>::value,
          "Not a valid AudioProgramme parameter");
      storage_.set(value);
    }

    ADM_EXPORT void set(AudioProgrammeId id);

    /**
     * @brief ADM parameter unset template
     *
     * Templated unset method with the ADM parameter type as template
     * argument. Removes an ADM parameter if it is optional or resets it to
     * the default value if there is one.
     */
    template <typename Parameter>
    void unset() {
      static_assert(
          AudioProgrammeParameterStore::isValidParameter<Parameter>::value,
          "Not a valid AudioProgramme parameter");
      storage_.unset<Parameter>();
    }

    /// @brief Add reference to an AudioContent
    ADM_EXPORT bool addReference(std::shared_ptr<AudioContent> content);

    /**
     * @brief Get references to ADM elements template
     *
     * Templated get method with the ADM parameter type as template
     * argument. Returns a set of all references to the ADM elements with the
     * specified type.
     */
    template <typename Element>
    ElementRange<Element> getReferences();

    /**
     * @brief Get references to ADM elements template
     *
     * Templated get method with the ADM parameter type as template
     * argument. Returns a set of all references to the ADM elements with the
     * specified type.
     */
    template <typename Element>
    ElementRange<const Element> getReferences() const;

    /// @brief Remove reference to an AudioContent
    ADM_EXPORT void removeReference(std::shared_ptr<AudioContent> content);

    /**
     * @brief Clear references to Elements template
     *
     * Templated clear method with the ADM parameter type as template
     * argument. Removes all references to the ADM elements with the specified
     * type.
     */
    template <typename Element>
    void clearReferences();

    /**
     * @brief Print overview to ostream
     */
    void print(std::ostream &os) const;

    /// Get adm::Document this element belongs to
    ADM_EXPORT std::weak_ptr<Document> getParent() const;

   private:
    friend class AudioProgrammeAttorney;

    ADM_EXPORT AudioProgramme(AudioProgrammeName name);
    ADM_EXPORT AudioProgramme(const AudioProgramme &) = default;
    ADM_EXPORT AudioProgramme(AudioProgramme &&) = default;

    ADM_EXPORT ElementRange<const AudioContent> getReferences(
        detail::ParameterTraits<AudioContent>::tag) const;

    ADM_EXPORT ElementRange<AudioContent> getReferences(
        detail::ParameterTraits<AudioContent>::tag);

    ADM_EXPORT void clearReferences(detail::ParameterTraits<AudioContent>::tag);

    ADM_EXPORT void disconnectReferences();

    void setParent(std::weak_ptr<Document> document);

    std::weak_ptr<Document> parent_;

    AudioProgrammeParameterStore storage_;
    std::vector<std::shared_ptr<AudioContent>> audioContents_;
  };
  ///@}

  // ---- Implementation ---- //

  template <typename... Parameters>
  std::shared_ptr<AudioProgramme> AudioProgramme::create(
      AudioProgrammeName name, Parameters... optionalNamedArgs) {
    std::shared_ptr<AudioProgramme> programme(new AudioProgramme(name));
    detail::setNamedOptionHelper(
        programme, std::forward<Parameters>(optionalNamedArgs)...);

    return programme;
  }

  template <typename Element>
  ElementRange<const Element> AudioProgramme::getReferences() const {
    typedef typename detail::ParameterTraits<Element>::tag Tag;
    return getReferences(Tag());
  }

  template <typename Element>
  ElementRange<Element> AudioProgramme::getReferences() {
    typedef typename detail::ParameterTraits<Element>::tag Tag;
    return getReferences(Tag());
  }

  inline ElementRange<const AudioContent> AudioProgramme::getReferences(
      detail::ParameterTraits<AudioContent>::tag) const {
    return detail::makeElementRange<AudioContent>(audioContents_);
  }

  inline ElementRange<AudioContent> AudioProgramme::getReferences(
      detail::ParameterTraits<AudioContent>::tag) {
    return detail::makeElementRange<AudioContent>(audioContents_);
  }

  template <typename Element>
  void AudioProgramme::clearReferences() {
    typedef typename detail::ParameterTraits<Element>::tag Tag;
    clearReferences(Tag());
  }

}  // namespace adm

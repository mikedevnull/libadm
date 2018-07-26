/// @file audio_content.hpp
#pragma once

#include <boost/optional.hpp>
#include <memory>
#include <vector>
#include "adm/elements/audio_content_id.hpp"
#include "adm/elements/audio_object.hpp"
#include "adm/elements/dialogue.hpp"
#include "adm/elements/loudness_metadata.hpp"
#include "adm/elements_fwd.hpp"
#include "adm/helper/element_range.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/property_store.hpp"
#include "adm/export.h"

namespace adm {

  class Document;

  /// @brief Tag for NamedType ::AudioContentName
  struct AudioContentNameTag {};
  /// @brief NamedType for the audioContentName attribute
  using AudioContentName = detail::NamedType<std::string, AudioContentNameTag>;
  /// @brief Tag for NamedType ::AudioContentLanguage
  struct AudioContentLanguageTag {};
  /// @brief NamedType for the language attribute of the
  /// audioContent element
  using AudioContentLanguage =
      detail::NamedType<std::string, AudioContentLanguageTag>;

  /// @brief Tag for AudioContent
  struct AudioContentTag {};

  struct AudioContentDefaults {
    using PropertiesWithDefaults = PropertyList<>;
  };
  /**
   * @brief Class representation of the audioContent ADM element
   *
   * @headerfile audio_content.hpp <adm/elements/audio_content.hpp>
   */
  class AudioContent : public std::enable_shared_from_this<AudioContent> {
    using ManditoryProperties = PropertyList<AudioContentId, AudioContentName>;
    using OptionalProperties =
        PropertyList<AudioContentLanguage, DialogueId, NonDialogueContentKind,
                     DialogueContentKind, MixedContentKind, LoudnessMetadata>;
    using AudioContentPropertyStore =
        PropertyStore<ManditoryProperties, OptionalProperties,
                      AudioContentDefaults>;

   public:
    typedef AudioContentTag tag;
    /// Type that holds the id for this element;
    typedef AudioContentId id_type;

    /**
     * @brief Static create function template
     *
     * Templated static create function which accepts a variable
     * number of ADM parameters in random order after the
     * mandatory ADM parameters. The actual constructor is
     * private. This way it is ensured, that an AudioContent
     * object can only be created as a `std::shared_ptr`.
     */
    template <typename... Parameters>
    static std::shared_ptr<AudioContent> create(
        AudioContentName name, Parameters... optionalNamedArgs);

    /**
     * @brief Copy AudioContent
     *
     * The actual copy constructor is private to ensure that an
     * AudioContent can only be created as a `std::shared_ptr`.
     * This is not a deep copy! All referenced objects will be
     * disconnected.
     */
    ADM_EXPORT std::shared_ptr<AudioContent> copy() const;

    /**
     * @brief ADM parameter getter template
     *
     * Templated getter with the wanted ADM parameter type as
     * template argument. If currently no value is available
     * trying to get the ADM parameter will result in an
     * exception. Check with the has method before
     */
    template <typename Parameter>
    Parameter get() const;

    /**
     * @brief ADM parameter has template
     *
     * Templated has method with the ADM parameter type as
     * template argument. Returns true if the ADM parameter is
     * set or has a default value.
     */
    template <typename Parameter>
    bool has() const;

    /**
     * @brief ADM parameter isDefault template
     *
     * Templated isDefault method with the ADM parameter type as
     * template argument. Returns true if the ADM parameter is
     * the default value.
     */
    template <typename Parameter>
    bool isDefault() const;

    template <typename Parameter>
    void set(const Parameter &);

    ADM_EXPORT void set(AudioContentId);
    ///@{

    /**
     * @brief Dialogue setter
     *
     * \note Setting one of the dialogue kinds always ensures
     * consistency. If
     * ::DialogueId is set the corresponding ContentKind will be
     * set to undefined. If one of the ContentKinds is set
     * ::DialogueId will be set accordingly.
     */
    ADM_EXPORT void set(DialogueId kind);
    ADM_EXPORT void set(ContentKind kind);
    ADM_EXPORT void set(NonDialogueContentKind kind);
    ADM_EXPORT void set(DialogueContentKind kind);
    ADM_EXPORT void set(MixedContentKind kind);
    ///@}

    /**
     * @brief ADM parameter unset template
     *
     * Templated unset method with the ADM parameter type as
     * template argument. Removes an ADM parameter if it is
     * optional or resets it to the default value if there is
     * one.
     *
     * \note Unsetting ::DialogueId automatically unsets the
     * corresponding ContentKind. Unsetting one of the
     * ContentKinds automatically unsets
     * ::DialogueId.
     */
    template <typename Parameter>
    void unset();

    /// @brief Add reference to an AudioObject
    ADM_EXPORT bool addReference(std::shared_ptr<AudioObject> object);

    /**
     * @brief Get references to ADM elements template
     *
     * Templated get method with the ADM parameter type as
     *template argument. Returns a set of all references to the
     *adm elements with the specified type.
     */
    template <typename Element>
    ElementRange<const Element> getReferences() const;

    /**
     * @brief Get references to ADM elements template
     *
     * Templated get method with the ADM parameter type as
     *template argument. Returns a set of all references to the
     *adm elements with the specified type.
     */
    template <typename Element>
    ElementRange<Element> getReferences();

    // template <typename Element>
    // ElementRange<const Element> getReferences()
    // const;

    /// @brief Remove reference to an AudioObject
    ADM_EXPORT void removeReference(std::shared_ptr<AudioObject> object);

    /**
     * @brief Clear references to Elements template
     *
     * Templated clear method with the ADM parameter type as
     *template argument. Removes all references to the adm
     *elements with the specified type.
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
    template <typename Type>
    struct DispatchTag {
      typedef Type T;
    };
    friend class AudioContentAttorney;

    ADM_EXPORT AudioContent(AudioContentName name);
    ADM_EXPORT AudioContent(const AudioContent &) = default;
    ADM_EXPORT AudioContent(AudioContent &&) = default;

    template <typename Tag>
    typename Tag::T get(Tag) const;

    ADM_EXPORT ContentKind get(DispatchTag<ContentKind>) const;

    template <typename Tag>
    bool has(Tag) const;

    ADM_EXPORT bool has(DispatchTag<ContentKind>) const;

    template <typename Tag>
    void unset(Tag);

    ADM_EXPORT void unset(DispatchTag<DialogueId>);
    ADM_EXPORT void unset(DispatchTag<ContentKind>);
    ADM_EXPORT void unset(DispatchTag<NonDialogueContentKind>);
    ADM_EXPORT void unset(DispatchTag<DialogueContentKind>);
    ADM_EXPORT void unset(DispatchTag<MixedContentKind>);

    ADM_EXPORT ElementRange<const AudioObject> getReferences(
        detail::ParameterTraits<AudioObject>::tag) const;

    ADM_EXPORT ElementRange<AudioObject> getReferences(
        detail::ParameterTraits<AudioObject>::tag);

    ADM_EXPORT void clearReferences(detail::ParameterTraits<AudioObject>::tag);

    ADM_EXPORT void disconnectReferences();

    void setParent(std::weak_ptr<Document> document);

    std::weak_ptr<Document> parent_;
    std::vector<std::shared_ptr<AudioObject>> audioObjects_;
    AudioContentPropertyStore storage_;
  };

  // ---- Implementation ---- //

  template <typename... Parameters>
  std::shared_ptr<AudioContent> AudioContent::create(
      AudioContentName name, Parameters... optionalNamedArgs) {
    std::shared_ptr<AudioContent> content(new AudioContent(name));
    detail::setNamedOptionHelper(
        content, std::forward<Parameters>(optionalNamedArgs)...);

    return content;
  }

  template <typename Parameter>
  Parameter AudioContent::get() const {
    return get(DispatchTag<Parameter>());
  }

  template <typename Tag>
  typename Tag::T AudioContent::get(Tag) const {
    typedef typename Tag::T Parameter;
    return storage_.get<Parameter>();
  }

  template <typename Parameter>
  bool AudioContent::has() const {
    return has(DispatchTag<Parameter>());
  }

  template <typename Tag>
  bool AudioContent::has(Tag) const {
    typedef typename Tag::T Parameter;
    return storage_.has<Parameter>();
  }

  template <typename Parameter>
  bool AudioContent::isDefault() const {
    return storage_.isDefault<Parameter>();
  }

  template <typename Parameter>
  void AudioContent::set(const Parameter &value) {
    storage_.set(value);
  }

  template <typename Parameter>
  void AudioContent::unset() {
    return unset(DispatchTag<Parameter>());
  }

  template <typename Tag>
  void AudioContent::unset(Tag) {
    typedef typename Tag::T Parameter;
    return storage_.unset<Parameter>();
  }

  template <typename Element>
  ElementRange<const Element> AudioContent::getReferences() const {
    typedef typename detail::ParameterTraits<Element>::tag Tag;
    return getReferences(Tag());
  }

  inline ElementRange<const AudioObject> AudioContent::getReferences(
      detail::ParameterTraits<AudioObject>::tag) const {
    return detail::makeElementRange<AudioObject>(audioObjects_);
  }

  template <typename Element>
  ElementRange<Element> AudioContent::getReferences() {
    typedef typename detail::ParameterTraits<Element>::tag Tag;
    return getReferences(Tag());
  }

  inline ElementRange<AudioObject> AudioContent::getReferences(
      detail::ParameterTraits<AudioObject>::tag) {
    return detail::makeElementRange<AudioObject>(audioObjects_);
  }

  template <typename Element>
  void AudioContent::clearReferences() {
    typedef typename detail::ParameterTraits<Element>::tag Tag;
    clearReferences(Tag());
  }

}  // namespace adm

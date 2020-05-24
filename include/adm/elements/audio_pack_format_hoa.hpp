/// @file audio_pack_format_hoa.hpp
#pragma once

#include "adm/elements/screen_ref.hpp"
#include "adm/elements/normalization.hpp"
#include "adm/elements/nfc_ref_dist.hpp"
#include "adm/elements/audio_pack_format.hpp"

namespace adm {

  class Document;
  struct AudioPackFormatHoaDefaults {
    ADM_EXPORT static Normalization create(Normalization::tag);
    ADM_EXPORT static NfcRefDist create(NfcRefDist::tag);
    ADM_EXPORT static ScreenRef create(ScreenRef::tag);
  };

  class AudioPackFormatHoa : public AudioPackFormat {
    using ParametersWithDefaults =
        ParameterList<ScreenRef, Normalization, NfcRefDist>;
    using AudioPackFormatHoaParameterStore =
        detail::ParameterStore<NoParameters, NoParameters,
                               ParametersWithDefaults,
                               AudioPackFormatHoaDefaults>;

   public:
    template <typename... Parameters>
    static std::shared_ptr<AudioPackFormatHoa> create(
        AudioPackFormatName name, Parameters... optionalNamedArgs);

    /**
     * @brief ADM parameter getter template
     *
     * Templated getter with the wanted ADM parameter type as template
     * argument. If currently no value is available trying to get the adm
     * parameter will result in an exception. Check with the has method before
     */
    template <typename Parameter>
    Parameter get() const;

    /**
     * @brief ADM parameter has template
     *
     * Templated has method with the ADM parameter type as template argument.
     * Returns true if the ADM parameter is set or has a default value.
     */
    template <typename Parameter>
    bool has() const;

    /**
     * @brief ADM parameter isDefault template
     *
     * Templated isDefault method with the ADM parameter type as template
     * argument. Returns true if the ADM parameter is the default value.
     */
    template <typename Parameter>
    bool isDefault() const;

    /**
     * @brief ADM parameter unset template
     *
     * Templated unset method with the ADM parameter type as template
     * argument. Removes an ADM parameter if it is optional or resets it to
     * the default value if there is one.
     */
    template <typename Parameter>
    void unset();

    template <typename Parameter>
    void set(const Parameter&);

    /**
     * @brief Print overview to ostream
     */
    void print(std::ostream& os) const;

   private:
    friend class AudioPackFormatHoaAttorney;
    using IsHoaParameterTag = std::true_type;
    using IsBasePackParameterTag = std::false_type;

    ADM_EXPORT AudioPackFormatHoa(AudioPackFormatName name);
    // ADM_EXPORT AudioPackFormatHoa(const AudioPackFormat &) = default;
    // ADM_EXPORT AudioPackFormatHoa(AudioPackFormat &&) = default;

    template <typename Parameter>
    Parameter getDispatch(IsBasePackParameterTag) const {
      return AudioPackFormat::get<Parameter>();
    }
    template <typename Parameter>
    Parameter getDispatch(IsHoaParameterTag) const {
      return storage_.get<Parameter>();
    }

    template <typename Parameter>
    bool hasDispatch(IsBasePackParameterTag) const {
      return AudioPackFormat::has<Parameter>();
    }
    template <typename Parameter>
    bool hasDispatch(IsHoaParameterTag) const {
      return storage_.has<Parameter>();
    }

    template <typename Parameter>
    bool isDefaultDispatch(IsBasePackParameterTag) const {
      return AudioPackFormat::isDefault<Parameter>();
    }
    template <typename Parameter>
    bool isDefaultDispatch(IsHoaParameterTag) const {
      return storage_.isDefault<Parameter>();
    }

    template <typename Parameter>
    void unsetDispatch(IsBasePackParameterTag) {
      AudioPackFormat::unset<Parameter>();
    }
    template <typename Parameter>
    void unsetDispatch(IsHoaParameterTag) {
      storage_.unset<Parameter>();
    }

    template <typename Parameter>
    void setDispatch(const Parameter& v, IsBasePackParameterTag) {
      AudioPackFormat::set(v);
    }
    template <typename Parameter>
    void setDispatch(const Parameter& v, IsHoaParameterTag) {
      storage_.set(v);
    }

    AudioPackFormatHoaParameterStore storage_;
  };

  // ---- Implementation ---- //
  template <typename... Parameters>
  std::shared_ptr<AudioPackFormatHoa> AudioPackFormatHoa::create(
      AudioPackFormatName name, Parameters... optionalNamedArgs) {
    std::shared_ptr<AudioPackFormatHoa> pack(new AudioPackFormatHoa(name));
    detail::setNamedOptionHelper(
        pack, std::forward<Parameters>(optionalNamedArgs)...);
    return pack;
  }

  template <typename Parameter>
  Parameter AudioPackFormatHoa::get() const {
    using isHoaParameter =
        AudioPackFormatHoaParameterStore::isValidParameter<Parameter>;
    return getDispatch<Parameter>(isHoaParameter());
  }

  template <typename Parameter>
  bool AudioPackFormatHoa::has() const {
    using isHoaParameter =
        AudioPackFormatHoaParameterStore::isValidParameter<Parameter>;
    return hasDispatch<Parameter>(isHoaParameter());
  }

  template <typename Parameter>
  bool AudioPackFormatHoa::isDefault() const {
    using isHoaParameter =
        AudioPackFormatHoaParameterStore::isValidParameter<Parameter>;
    return isDefaultDispatch<Parameter>(isHoaParameter());
  }

  template <typename Parameter>
  void AudioPackFormatHoa::unset() {
    using isHoaParameter =
        AudioPackFormatHoaParameterStore::isValidParameter<Parameter>;
    return unsetDispatch<Parameter>(isHoaParameter());
  }
  template <typename Parameter>
  void AudioPackFormatHoa::set(const Parameter& v) {
    using isHoaParameter =
        AudioPackFormatHoaParameterStore::isValidParameter<Parameter>;
    setDispatch(v, isHoaParameter());
  }
}  // namespace adm

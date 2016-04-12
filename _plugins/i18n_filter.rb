require 'i18n'

LOCALE = Jekyll.configuration({})['default_lang'] # set your locale from config var

# Create folder "_locales" and put some locale file from https://github.com/svenfuchs/rails-i18n/tree/master/rails/locale
module Jekyll
  module I18nFilter
    # Example:
    #   {{ post.date | localize: "%d.%m.%Y" }}
    #   {{ post.date | localize: ":short" }}
    def localize(input, format=nil, custom_locale = LOCALE)
      load_translations(custom_locale)
      format = (format =~ /^:(\w+)/) ? $1.to_sym : format
      I18n.l input, :format => format
    rescue
      "error"
    end

    def load_translations(custom_locale)
      if I18n.backend.send(:translations).empty?
        I18n.backend.load_translations Dir[File.join(File.dirname(__FILE__),'locales/*.yml')]
        I18n.locale = custom_locale;
      end
    end
  end
end

Liquid::Template.register_filter(Jekyll::I18nFilter)

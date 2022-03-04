
class GaUtils
  def self.os
    RUBY_PLATFORM
  end

  def self.is_osx?
    check_os(os, osx_pattern)
  end

  def self.is_linux?
    check_os(os, linux_pattern)
  end

  def self.check_os(str_os, pattern)
    not (str_os =~ pattern).nil?
  end

  def self.sed
    case os
    when osx_pattern
      'gsed'
    when linux_pattern
      'sed'
    else
      throw 'what os is this'
    end
  end

  private

  def self.osx_pattern
    /darwin/
  end
  def self.linux_pattern
    /linux/
  end

end

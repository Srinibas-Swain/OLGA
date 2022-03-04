require 'string_ext'
require 'hash_ext'
require 'ga_utils'

class Template
  attr_accessor :class_name, :params, :file_name, :str_guard

  def initialize(_c_class_name)
    @c_class_name = _c_class_name
    compute_params()
  end

  def write_out(path_template, path_out, params)
	  inject_variables()
    str_template = read_template(path_template)
    str_out = parse_template(str_template, params)
    write_to_file(path_out, str_out)
  end

  private

  def read_template(path_template)
    File.read(File.expand_path('../../'+path_template, __FILE__))
  end

  def parse_template(str_template, params)
    tokens = get_tokens(str_template)
	puts("params: #{params}")
	puts("tokens: #{tokens}")
	puts("--"*7)
    tokens.each do |token|
		#validate_token(token, params)
		puts("token: #{token}")
	  token_val = eval('@'+token)
	  token_escaped = escape_characters_in_string(token)
	  puts("token: #{token} token_esc: #{token_escaped} value: #{token_val}")
	  str_template.gsub!(/#\{#{token_escaped}\}/, token_val)
    end
    str_template
  end

  def validate_token(token, params)
    if params[token].nil?
      raise("params has no value for token #{token}")
    end
  end

  def get_tokens(str_template)
    str_template.scan(/#\{([^:} ]*)\}/).uniq.map(&:first)
  end

  def write_to_file(path_out, str_out)
    File.open(corrected_path(path_out), 'w+') do |f|
      f.write(str_out)
    end
  end

  def corrected_path(path_out)
    File.expand_path('../../../'+path_out, __FILE__)
  end


  def compute_params()
    @params = {}

	
	@file_name = @c_class_name.underscore
	@params['class_name'] = @c_class_name
	@params['str_guard'] = "#{@file_name.upcase}_HH"
    @params['file_name'] = @file_name
	inject_variables()
	
	# @str_guard = @params['str_guard']
	# @class_name = @params['class_name']
  end

  def inject_variables()
	  puts("injecting variables ... ")
	  @params.each do |key, value|
		  puts("#{key} => #{value}")
		  instance_variable_set "@#{key}", value
	  end
	  puts('--'*10)
  end

  def escape_characters_in_string(string)
      pattern = /(\'|\"|\[|\]|\*|\/|\\)/
      string.gsub(pattern){|match|"\\"  + match} # <-- Trying to take the currently found match and add a \ before it I have no idea how to do that).
  end

end

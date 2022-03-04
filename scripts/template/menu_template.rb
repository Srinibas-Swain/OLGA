load 'template.rb'

class MenuTemplate < Template
	attr_accessor :file_name

	def execute(fo_path, should_update_welcome=true)
		inject_variables()
		write_header(fo_path)
		write_src(fo_path)
		write_test()
		update_makefile()
		update_welcome() if should_update_welcome
	end

	def write_header(fo_path='templates/menu_option')
		write_out(fo_path + '/header.template',
				  "include/#{file_name}.hh",
				  @params)
	end

	def write_src(fo_path='templates/menu_option')
		params['file_name'] = file_name
		write_out(fo_path + '/src.template',
				  "src/#{file_name}.cc",
				  @params)
	end

	def write_test()
		`touch tests/#{@file_name}_test.cc`
	end

	def update_makefile()
		`#{GaUtils.sed} '/^SRC=/ s/$/ #{@file_name}/' Makefile -i`
	end

	def update_welcome()
		`#{GaUtils.sed} '/MENU_OPTION_HEADERS::END/i #include "#{@file_name}.hh"' welcome.cc -i`
		`#{GaUtils.sed} '/MENU_OPTIONS::END/i \\\toptions.push_back(new #{@c_class_name}());' welcome.cc -i`
	end

	private
	def compute_params
		super
		title = file_name.split('_').map(&:capitalize).join(' ')
		@params['title'] = title

		object_name = file_name.split('_').map{|word| word[0]}.join()
		@params['object_name'] = object_name
		puts(@params)
	end
end



require 'regular_template.rb'
require 'menu_template.rb'

class TemplateCreator
	def self.class_selector(c_class_type, c_class_name)
		case c_class_type.upcase
		when 'REGULAR'
			{klass: RegularTemplate,
			 items: [
				 {class_name: c_class_name,
				  template_folder: 'templates/regular',
				  update_makefile: true
				 }]}
		when 'MENU'
			{klass: MenuTemplate,
			 items: [
				 {class_name: c_class_name, 			template_folder: 'templates/menu_option',		 update_welcome: true},
				 {class_name: c_class_name+'Helper', 	template_folder: 'templates/menu_option_helper', update_welcome: false}]
			}
		else
			raise("Invalid Class Type: #{c_class_type}")
		end
	end

	def self.create(c_class_type, c_class_name)
		klass_info = class_selector(c_class_type, c_class_name)
		klass = klass_info[:klass]
		klass_info[:items].each do |specs|
			klass.new(specs[:class_name]).execute(specs[:template_folder], specs[:update_welcome])
		end
	end
end


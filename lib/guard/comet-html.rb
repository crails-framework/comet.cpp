module ::Guard
  class CometHtml < Plugin
    def initialize options = {}
      @options = options
      super
    end

    def run_on_modifications(paths)
      run_all
    end

    def generate_command
      gemspec  = Gem::Specification.find_all_by_name("comet-cpp").first
      command  = "#{gemspec.bin_dir}/comet-html"
      command += " -i '#{@options[:source]}'" unless @options[:source].nil?
      command += " -o '#{@options[:output]}'" unless @options[:output].nil?
      command += " -c '#{@options[:config]}'" unless @options[:config].nil?
      command
    end

    def run_all
      PTY.spawn(generate_command) do |stdout,stdin,pid|
        begin
          stdout.each {|line| print line}
        rescue Errno::EIO
        end
        Process.wait(pid)
      end
      $?.success?
    end
  end
end

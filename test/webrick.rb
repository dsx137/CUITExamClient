#!/usr/bin/env ruby
 
require 'webrick'


Dir.chdir(File.dirname(__FILE__))

class Post < WEBrick::HTTPServlet::AbstractServlet
  def do_POST(request, response)
    # 以utf8保存request.body到log.txt
    File.open("./log.txt", "wb") do |f|
    f.write(request)
    f.close
    end

    data = request.body
    # get file from multipart form data
    image = data[/filename="(.*)"/m, 1]
    image = data[/\r\n\r\n(.*)\z/m, 1]
   
    
    File.open("./image.jpg", "wb") do |f|
    f.write(image)
    f.close
    end

    
    
    # set the response status and content type
    response.status = 200
    response.content_type = "text/plain"
    
    # send a message back to the client
    response.body = "Image received successfully!"
  end
end


webroot = File.expand_path ''

# create a server object
server = WEBrick::HTTPServer.new(:Port => 8000, :DocumentRoot => webroot)



# mount the Post servlet at the root path (/)
server.mount("/Img", Post)
trap 'INT' do server.shutdown end
server.start
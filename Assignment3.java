import java.net.*;
import java.io.*;
import java.nio.charset.StandardCharsets;
import org.jsoup.*;
import org.jsoup.nodes.Document;
import org.jsoup.nodes.Element;
import org.jsoup.select.Elements;
import java.util.HashMap;

public class Assignment3 {
	public static void main(String args[]){
		Document doc;
		try{
			File input = new File("listOfSolarSysObjectsBySize.html");
			doc = Jsoup.parse(input, "UTF-8");
			Elements oltags = doc.getElementById("content").getElementsByTag("ol");
			oltags = oltags.attr("class", "references");
			//System.out.println(oltags);
			HashMap<String, String> tagmap = new HashMap<String, String>();
			for(Element ol : oltags){
				for(Element li : ol.children()){
					Element a = li.getElementsByTag("a").first();
					String href =  a.attr("href");
					tagmap.put(href.substring(1),li.text());
					
				//System.out.println("href equals: (" + href + "}");
					//System.out.print(id);
					
					//System.out.println("li text: " + li.text());
				}
			}
			
			//System.out.println(tagmap);
			//for each "li" tag, get teh cite note then what they wrote, put on hashmap
			Element table = doc.getElementsByTag("table").get(1);
			
			Elements trs = table.getElementsByTag("tr");
			//System.out.println(trs);
			for(Element tr : trs){
				Elements tds = tr.getElementsByTag("td");
				for(Element td : tds){
					Elements sups = td.getElementsByTag("sup");
					if(sups.size() != 0){
						Element sup = sups.first();
						String supId = sup.id();
						
						if(tagmap.get(supId) != null)
						{
						System.out.println(td.text() + " ref " + tagmap.get(supId));
						}
						else{
						System.out.println(td.text() + " ref " + supId);
						}
						
					}
					else{
						System.out.println(td.text());
					}
				}
				//System.out.println(tr);
				//break;
			}
		}
		catch(IOException ex){
			System.out.println(ex.toString());
		}
	}
}

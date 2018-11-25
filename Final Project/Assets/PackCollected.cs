using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class PackCollected : MonoBehaviour {
    public PlayerControls player;
    public Text packTracker;
    public int packsNeeded;
    //public Canvas 
	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		
	}

    void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "Player")
        {
            player.packsCollected++;
            packTracker.text = "Energy Packs: " + player.packsCollected + "/" + packsNeeded;
            Destroy(this.gameObject);
        }    
    }
}

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameManager : MonoBehaviour {
    public bool gameHasEnded = false;
    public float restartDelay = 2f;
    public GameObject completeLevelUI;

    public void CompleteLevel()
    {
        Debug.Log("Level complete!");
        completeLevelUI.SetActive(true);
    }

}
